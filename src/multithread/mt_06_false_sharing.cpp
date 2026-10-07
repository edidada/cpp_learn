// mt_06_false_sharing.cpp —— 硬件层：cache line、伪共享与扩展性
//
// 对应 docs/multy_thread.md：
//   二、第一层硬件（Cache / Cache Line / MESI / Store Buffer / False Sharing）
//   二十三、性能树（Contention / Cache Line Ping-Pong / Scalability）
//
// 关键认知：伪共享**不是正确性问题**，加锁、用原子都救不了它；
// 它是缓存一致性协议按"线"而不是按"字节"同步导致的性能问题。
// 8 个线程各写自己的变量，变量却挤在同一条 64B 线上 -> 每次写都要抢回独占权（RFO）。
#include "mt_common.h"

#if MT_HAS_11
#include <atomic>
#include <thread>
#include <vector>
#endif

namespace {

const int kThreads = 8;
const int kIters   = 1000000;

// --- 版本 A：8 个计数器挤在同一条（或两条）cache line 上 ---
struct Packed {
    long counters[kThreads];
};

// --- 版本 B：每个计数器独占一条 cache line ---
struct MT_ALIGNED64 Slot {
    long v;
};
struct Padded {
    Slot slots[kThreads];
};

Packed  g_packed;
Padded  g_padded;

// C++98 路径：原子自增（保证正确）但位置决定性能
void bump_packed(void* arg) {
    int id = (int)mt::from_ptr(arg);
    for (int i = 0; i < kIters; ++i) mt::os_fetch_add(&g_packed.counters[id], 1);
}
void bump_padded(void* arg) {
    int id = (int)mt::from_ptr(arg);
    for (int i = 0; i < kIters; ++i) mt::os_fetch_add(&g_padded.slots[id].v, 1);
}

// --- 争用扩展性：所有线程抢同一个变量 ---
long g_shared = 0;
int  g_burn = 1;      // 每个线程在临界区外做的"本地活"，模拟真实计算

void shared_worker(void*) {
    for (int i = 0; i < 100000; ++i) {
        mt::os_fetch_add(&g_shared, 1);
        for (int b = 0; b < g_burn; ++b) { /* 本地计算，不碰共享值 */ }
    }
}

#if MT_HAS_11
// C++11 版：relaxed 原子自增，配合 alignas 的数组
// 注意：std::atomic 不可复制/移动，所以这里用定长数组，不能用 std::vector
struct alignas(64) AtomicSlot { std::atomic<long> v; };
AtomicSlot g_atomic_slots[kThreads];
std::atomic<long> g_atomic_one;

void atomic_relaxed_worker(int id) {
    for (int i = 0; i < kIters / 4; ++i) {
        g_atomic_slots[id].v.fetch_add(1, std::memory_order_relaxed);
    }
}
void atomic_contended_worker() {
    for (int i = 0; i < kIters / 4; ++i) {
        g_atomic_one.fetch_add(1, std::memory_order_relaxed);
    }
}
#endif

double run_packed() {
    double t0 = mt::now_ms();
    mt::Thread ts[kThreads];
    for (int i = 0; i < kThreads; ++i) ts[i].start(&bump_packed, mt::to_ptr(i));
    for (int i = 0; i < kThreads; ++i) ts[i].join();
    return mt::now_ms() - t0;
}

double run_padded() {
    double t0 = mt::now_ms();
    mt::Thread ts[kThreads];
    for (int i = 0; i < kThreads; ++i) ts[i].start(&bump_padded, mt::to_ptr(i));
    for (int i = 0; i < kThreads; ++i) ts[i].join();
    return mt::now_ms() - t0;
}

}  // namespace

int main() {
    std::cout << "mt_06_false_sharing : coherence granularity, not correctness\n";
    std::cout << "  cache line assumed = " << (long long)mt::cache_line() << " bytes\n";
    std::cout << "  threads = " << kThreads << ", each does " << kIters << " increments\n";

    mt_section(1, "same logical work, different memory layout");
    for (int i = 0; i < kThreads; ++i) { g_packed.counters[i] = 0; g_padded.slots[i].v = 0; }
    double t_pack = run_packed();
    double t_pad  = run_padded();
    long sum_pack = 0, sum_pad = 0;
    for (int i = 0; i < kThreads; ++i) { sum_pack += g_packed.counters[i]; sum_pad += g_padded.slots[i].v; }
    std::cout << "  packed  : " << t_pack << " ms, sum = " << sum_pack << "\n";
    std::cout << "  padded  : " << t_pad  << " ms, sum = " << sum_pad  << "\n";
    std::cout << "  speedup = " << (t_pack / (t_pad > 0 ? t_pad : 1)) << "x\n";
    std::cout << "  两个版本的结果完全一样，差的是缓存行所有权来回迁移的次数。\n";

    mt_section(23, "contention scaling: one hot line vs N private lines");
    for (int n = 1; n <= kThreads; n <<= 1) {
        g_shared = 0; g_burn = 50;
        mt::Thread ts[8];
        double t0 = mt::now_ms();
        for (int i = 0; i < n; ++i) ts[i].start(&shared_worker, NULL);
        for (int i = 0; i < n; ++i) ts[i].join();
        double ms = mt::now_ms() - t0;
        std::cout << "  threads=" << n << " total ops=" << g_shared
                  << " time=" << ms << " ms ops/ms=" << (g_shared / (ms > 0 ? ms : 1)) << "\n";
    }
    std::cout << "  Amdahl 的意思就在这儿：共享点越多，加核带来的收益越早见顶。\n";

#if MT_HAS_11
    mt_section(1, "std::atomic + alignas(64): the standard-library shape of the same fix");
    for (int i = 0; i < kThreads; ++i) g_atomic_slots[i].v.store(0);
    g_atomic_one.store(0);
    {
        double t0 = mt::now_ms();
        std::vector<std::thread> v;
        for (int i = 0; i < kThreads; ++i) v.push_back(std::thread(&atomic_relaxed_worker, i));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        long total = 0;
        for (int i = 0; i < kThreads; ++i) total += g_atomic_slots[i].v.load();
        std::cout << "  per-thread atomic (alignas 64): " << (mt::now_ms() - t0)
                  << " ms, total = " << total << "\n";
    }
    {
        double t0 = mt::now_ms();
        std::vector<std::thread> v;
        for (int i = 0; i < kThreads; ++i) v.push_back(std::thread(&atomic_contended_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  single shared atomic          : " << (mt::now_ms() - t0)
                  << " ms, total = " << g_atomic_one.load() << "\n";
    }
    std::cout << "  relaxed 已经把顺序开销降到最低，剩下的差距几乎全是缓存行迁移。\n";
#else
    mt_skip(11, "std::atomic<long> + alignas(64)",
            "alignas 与语言级原子都是 C++11；上面已用 __attribute__((aligned(64))) 做等价演示");
#endif

    std::cout << "\npractical checklist:\n"
                 "  - 每线程计数器/统计量：独占 cache line，最后再归约\n"
                 "  - 无锁队列的 head/tail：分到不同线，或干脆各占一条\n"
                 "  - 结构体里热字段与冷字段分开排布\n"
                 "  - 测出来再说：核少、竞争弱时，伪共享可能根本看不见\n";
    return 0;
}
