// mt_02_memory_orders.cpp —— 可见性与顺序性：memory_order 到底在管什么
//
// 对应 docs/multy_thread.md：
//   二、第一层硬件 —— Store Buffer 让"写"和"读"不再按源码顺序被别人看到
//   三、第二层 ISA —— LFENCE/SFENCE/MFENCE、LDAR/STLR
//   四、第三层 Memory Model —— happens-before / acquire / release / seq_cst
//
// 两个实验：
//   A. 消息传递（data + flag）：release store / acquire load 建立同步关系
//      C++98 也能跑：用编译器内置屏障 os_fence() 得到同样的效果。
//   B. Dekker 模式（store X; load Y）：x86 允许 StoreLoad 乱序，
//      所以"双方都看到对方的写还是 0"在 relaxed 下真能观察到；seq_cst 不允许。
#include "mt_common.h"

#if MT_HAS_11
#include <atomic>
#include <thread>
#endif

namespace {

// ---------------------------------------------------------------------------
// A. 消息传递：写数据 -> 写标志 -> 对方读标志 -> 读数据
// ---------------------------------------------------------------------------
volatile long s_data = 0;      // "业务数据"
volatile long s_flag = 0;      // "发布通知"

void producer_os(void*) {
    s_data = 20261007;
    // release 语义：前面的写必须先对其他 CPU 可见，才允许 flag 的写逃逸出去。
    // 没有这一条屏障，弱内存序机器（ARM/RISC-V）上消费者可能看到 flag=1 但 data 还是旧值。
    mt::os_fence();
    s_flag = 1;
}

void consumer_os(void*) {
    while (s_flag == 0) { /* spin：等发布 */ }
    mt::os_fence();            // acquire 语义：之后的读不能被提到 flag 读之前
    std::cout << "  [os fence]  consumer sees data = " << s_data
              << "   (must be 20261007)\n";
}

#if MT_HAS_11
std::atomic<long> a_data;
std::atomic<long> a_flag;

void producer_relaxed() {
    // relaxed：只保证这一句自身是原子可见的，不提供任何顺序
    a_data.store(20261007, std::memory_order_relaxed);
    a_flag.store(1, std::memory_order_relaxed);
}

void producer_release() {
    a_data.store(20261007, std::memory_order_relaxed);
    a_flag.store(1, std::memory_order_release);   // 发布点
}

void consumer_acquire() {
    long f = 0;
    int spins = 0;
    while (spins++ < 50000000) {
        f = a_flag.load(std::memory_order_acquire);   // 获取点
        if (f == 1) break;
    }
    if (f == 1) {
        std::cout << "  [release/acquire] consumer sees data = "
                  << a_data.load(std::memory_order_relaxed) << "\n";
    } else {
        std::cout << "  [release/acquire] gave up waiting\n";
    }
}

void consumer_relaxed() {
    long f = 0;
    int spins = 0;
    while (spins++ < 50000000) {
        f = a_flag.load(std::memory_order_relaxed);
        if (f == 1) break;
    }
    std::cout << "  [relaxed/relaxed] data = "
              << a_data.load(std::memory_order_relaxed)
              << "  —— 结果无保证，可能读到旧值，这就是 unsafe publication\n";
}

// ---------------------------------------------------------------------------
// B. Dekker：store X 与 load Y 之间的乱序
// ---------------------------------------------------------------------------
std::atomic<long> d_x, d_y;
std::atomic<long> d_go, d_done;
long d_result_r1 = 0, d_result_r2 = 0;
bool d_use_seq_cst = true;

void daker_thread_a(int rounds) {
    for (int r = 1; r <= rounds; ++r) {
        while (d_go.load(std::memory_order_seq_cst) < r) { /* 等主线程开下一轮 */ }
        if (d_use_seq_cst) {
            d_x.store(1, std::memory_order_seq_cst);
            d_result_r1 = d_y.load(std::memory_order_seq_cst);
        } else {
            d_x.store(1, std::memory_order_relaxed);
            d_result_r1 = d_y.load(std::memory_order_relaxed);
        }
        d_done.fetch_add(1, std::memory_order_acq_rel);
    }
}

void daker_thread_b(int rounds) {
    for (int r = 1; r <= rounds; ++r) {
        while (d_go.load(std::memory_order_seq_cst) < r) { }
        if (d_use_seq_cst) {
            d_y.store(1, std::memory_order_seq_cst);
            d_result_r2 = d_x.load(std::memory_order_seq_cst);
        } else {
            d_y.store(1, std::memory_order_relaxed);
            d_result_r2 = d_x.load(std::memory_order_relaxed);
        }
        d_done.fetch_add(1, std::memory_order_acq_rel);
    }
}

// 统计"两个线程都看到对方仍是 0"的次数：即 StoreLoad 被重排的证据
int run_dekker(int rounds, bool seq_cst) {
    d_use_seq_cst = seq_cst;
    d_x.store(0); d_y.store(0); d_go.store(0); d_done.store(0);

    std::thread ta(&daker_thread_a, rounds);
    std::thread tb(&daker_thread_b, rounds);

    int both_zero = 0;
    for (int r = 1; r <= rounds; ++r) {
        d_x.store(0, std::memory_order_seq_cst);
        d_y.store(0, std::memory_order_seq_cst);
        d_go.store(r, std::memory_order_seq_cst);
        while (d_done.load(std::memory_order_seq_cst) < 2 * r) { /* 等这一轮结束 */ }
        if (d_result_r1 == 0 && d_result_r2 == 0) ++both_zero;
    }
    ta.join();
    tb.join();
    return both_zero;
}
#endif  // MT_HAS_11

}  // namespace

int main() {
    std::cout << "mt_02_memory_orders : visibility and ordering\n";

    // ---- A. C++98 也能跑：内置屏障版消息传递 ----
    mt_section(3, "message passing with compiler-level fences (works at C++98)");
    s_data = 0; s_flag = 0;
    {
        mt::Thread consumer, producer;
        consumer.start(&consumer_os, NULL);   // 先起消费者，制造真实的等待
        producer.start(&producer_os, NULL);
        producer.join();
        consumer.join();
    }

#if MT_HAS_11
    // ---- A'. 同一件事的标准库写法 ----
    mt_section(3, "the same thing with memory_order_release / acquire");
    a_data.store(0); a_flag.store(0);
    {
        std::thread t1(&consumer_acquire), t2(&producer_release);
        t1.join(); t2.join();
    }

    mt_section(3, "relaxed + relaxed = no synchronizes-with edge");
    a_data.store(0); a_flag.store(0);
    {
        std::thread t1(&consumer_relaxed), t2(&producer_relaxed);
        t1.join(); t2.join();
        std::cout << "  relaxed 不报错也不崩，只是标准不再保证你能看到什么。\n";
        std::cout << "  它适合纯计数（比如统计 QPS），不适合发布数据。\n";
    }

    // ---- B. StoreLoad 乱序 ----
    mt_section(1, "Dekker pattern: store-buffer reordering, measured");
    const int kRounds = 200000;
    std::cout << "  rounds = " << kRounds << ", build = " << mt::mt_opt_name() << "\n";
    int relaxed_hits = run_dekker(kRounds, false);
    int seqcst_hits  = run_dekker(kRounds, true);
    std::cout << "  [relaxed ] r1==0 && r2==0 observed " << relaxed_hits << " times"
              << "  (" << (100.0 * relaxed_hits / kRounds) << "% of rounds)\n";
    std::cout << "  [seq_cst ] r1==0 && r2==0 observed " << seqcst_hits << " times\n";
    std::cout << "  说明：x86 只允许 StoreLoad 这一种重排，所以 relaxed 版本真能抓到；\n"
                 "        seq_cst 会插 mfence/lock 前缀，把这条口子堵死（应为 0）。\n"
                 "        ARM/RISC-V 上四种乱序都可能，弱内存序才是 memory_order 的主战场。\n";
    if (relaxed_hits == 0) {
        std::cout << "  本次 relaxed 也是 0：多半是 -O0 编译的（看上面 build 那行）。\n"
                  "        未优化的代码把 store buffer 窗口冲得七零八落，现象复现不出来，\n"
                  "        这不代表重排不存在。用 cmake/multithread（自带 -O2）重跑即可。\n";
    }
#else
    mt_skip(11, "std::atomic memory_order / Dekker experiment",
            "语言级 memory_order 是 C++11 引入的；上面 os_fence() 那一版是它在 C++98 的手工替身");
#endif
    return 0;
}
