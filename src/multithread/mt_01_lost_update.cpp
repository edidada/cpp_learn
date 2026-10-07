// mt_01_lost_update.cpp —— 原子性缺失：lost update 从机器层长什么样
//
// 对应 docs/multy_thread.md：
//   三、第二层 ISA/汇编 —— "counter++ 是三条指令"
//   四、第三层 Memory Model —— Atomicity / Visibility / Ordering
//   七、第六层 并发正确性 —— Lost Update / Data Race
//
// 三个版本跑同一件事：N 个线程各自增 M 次，正确答案 N*M。
//   v0 普通 long          -> 结果可能小于 N*M（lost update，且形式上是 data race = UB）
//   v1 OS 原子 fetch_add  -> 恒定正确，C++98 就能跑
//   v2 std::atomic        -> C++11 起的标准写法，同一个 lock xadd
//
// 注意：v0 严格来说是未定义行为。这里刻意只用 -O0/-O1 下的"单字长对齐写"，
// 让乱序可见而不炸，纯粹为了观察到 lost update；不要在生产代码里模仿。
#include "mt_common.h"

#if MT_HAS_11
#include <atomic>
#include <thread>
#include <vector>
#endif

namespace {

const int kThreads = 8;
const int kLoops   = 200000;

// v0：非原子自增。counter++ 在机器层是 load / add / store 三步，
// 两个线程可能都读到同一个旧值，于是两次自增只写回一次 —— 丢失更新。
//
// 这里故意给两个版本，因为「非原子」其实有两层错：
//   g_plain_counter     ：不加 volatile。-O2 下 GCC 直接把 20 万次 ++ 强度削弱成
//                          一条 `addl $200000, counter(%rip)`（实测汇编就是这样，
//                          它仍然是普通非原子 RMW）。八条这样的指令在时间上被线程
//                          启动间隔拉开了，于是结果常常"碰巧对" —— 别被骗。
//   g_volatile_counter  ：加 volatile。禁止编译器缓存与合并，每一轮都是真实的
//                          load -> add -> store，于是丢失更新每次都稳定复现。
//                          这正好说明：volatile 管的是"别缓存"（可见性的一半），
//                          它完全不提供原子性 —— 这才是 C++11 要发明 atomic 的理由。
long g_plain_counter = 0;
volatile long g_volatile_counter = 0;

void plain_worker(void*) {
    for (int i = 0; i < kLoops; ++i) {
        g_plain_counter = g_plain_counter + 1;   // 非原子 RMW
    }
}

void volatile_worker(void*) {
    for (int i = 0; i < kLoops; ++i) {
        g_volatile_counter = g_volatile_counter + 1;   // 每轮真的读写内存，照样非原子
    }
}

// v1：编译器内置原子（GCC __sync / MSVC Interlocked），最终是一条 lock 前缀指令。
long g_os_counter = 0;

void os_atomic_worker(void*) {
    for (int i = 0; i < kLoops; ++i) {
        mt::os_fetch_add(&g_os_counter, 1);      // 原子 RMW
    }
}

#if MT_HAS_11
// v2：标准库原子。fetch_add 默认 memory_order_relaxed —— 保证原子性，不保证顺序。
std::atomic<long> g_std_counter;

void std_atomic_worker() {
    for (int i = 0; i < kLoops; ++i) {
        g_std_counter.fetch_add(1, std::memory_order_relaxed);
    }
}
#endif

}  // namespace

int main() {
    std::cout << "mt_01_lost_update : atomicity is the first concurrency problem\n";
    const long expected = (long)kThreads * (long)kLoops;
    std::cout << "  expected = threads(" << kThreads << ") x loops(" << kLoops
              << ") = " << expected << "\n";

    // ---- v0 非原子 ----
    mt_section(6, "plain counter++ -> lost update");
    std::cout << "  build = " << mt::mt_opt_name() << "\n";
    g_plain_counter = 0;
    g_volatile_counter = 0;
    {
        mt::Thread ts[kThreads];
        for (int i = 0; i < kThreads; ++i) ts[i].start(&plain_worker, NULL);
        for (int i = 0; i < kThreads; ++i) ts[i].join();
    }
    std::cout << "  plain (no volatile)    result = " << g_plain_counter
              << "   lost = " << (expected - (long)g_plain_counter)
              << "   (-O2 把循环合成一条非原子 addl，看起来对是运气)\n";
    {
        mt::Thread ts[kThreads];
        for (int i = 0; i < kThreads; ++i) ts[i].start(&volatile_worker, NULL);
        for (int i = 0; i < kThreads; ++i) ts[i].join();
    }
    std::cout << "  plain (volatile)       result = " << g_volatile_counter
              << "   lost = " << (expected - (long)g_volatile_counter)
              << "   (nondeterministic, run again)\n";
    std::cout << "  两行演示的是同一件事的两个环节：\n"
                 "    编译器改写（可见性）  -> 上一行被强度削弱，数字可能\"碰巧对\"，那是巧合\n"
                 "    非原子 RMW（原子性）  -> 下一行强制每轮真的读写内存，必然丢\n";

    // ---- v1 OS 原子 ----
    mt_section(2, "compiler intrinsic atomic = one LOCK-prefixed instruction");
    g_os_counter = 0;
    {
        mt::Thread ts[kThreads];
        for (int i = 0; i < kThreads; ++i) ts[i].start(&os_atomic_worker, NULL);
        for (int i = 0; i < kThreads; ++i) ts[i].join();
    }
    std::cout << "  os fetch_add result = " << g_os_counter
              << "   correct = " << (g_os_counter == expected ? "yes" : "no") << "\n";
    std::cout << "  backend: " << MT_OS_ATOMIC_NAME << "\n";

    // ---- v2 std::atomic ----
#if MT_HAS_11
    mt_section(3, "std::atomic, the C++11 way");
    g_std_counter.store(0);
    {
        std::vector<std::thread> v;
        for (int i = 0; i < kThreads; ++i) v.push_back(std::thread(&std_atomic_worker));
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
        std::cout << "  std::atomic result = " << g_std_counter.load()
                  << "   correct = "
                  << (g_std_counter.load() == expected ? "yes" : "no") << "\n";
#if MT_HAS_17
        std::cout << "  is_always_lock_free = "
                  << (std::atomic<long>::is_always_lock_free ? "true" : "false")
                  << "  (false 时库会退化成内部自旋锁)\n";
#else
        std::cout << "  is_lock_free() = "
                  << (g_std_counter.is_lock_free() ? "true" : "false")
                  << "  (false 时库用内部自旋锁模拟这个原子对象)\n";
#endif
    }
#else
    mt_skip(11, "std::atomic<long>",
            "语言层原子类型 C++11 才有；本分支上 v1 的内置原子就是等价手段");
#endif

    // ---- 原子性还不够：复合操作照样错 ----
    mt_section(6, "atomicity violation on a compound operation (check-then-act)");
    std::cout << "  int a = 0, b = 1;\n"
                 "  T1: if (a == 0) b = 2;   T2: if (b == 1) a = 3;\n"
                 "  两个分支各自都是原子的，但中间可以被插队 -> 终态 a=0,b=1。\n"
                 "  结论：原子指令只保护单个对象的一次 RMW，不保护'逻辑上的一段'。\n"
                 "  这段的可运行版本见 mt_04_correctness.cpp。\n";
    return 0;
}
