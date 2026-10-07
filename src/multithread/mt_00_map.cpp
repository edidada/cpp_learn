// mt_00_map.cpp —— 并发/异步知识地图 + 当前分支能力矩阵
//
// 对应 docs/multy_thread.md 第一节"先建立总地图"、二十四节"最终形成四条主线"。
//
// 这个文件是整个 src/multithread 系列的索引：
//   1) 打印 12 层地图，标出每一层由哪个演示文件负责；
//   2) 打印当前语言级别 + 标准库能力探测结果（__cpp_lib_* 特性宏）；
//   3) 说明"能力探测 + #if 惰性分支"这套写法本身，就是低标准分支往高标准
//      分支 merge 时能共用同一份代码的原因。
//
// 任何标准（C++98 ~ C++26）下都能编译运行。
#include "mt_common.h"

// 库设施必须"包含对应头文件"以后特性宏才有定义
#if MT_HAS_11
#include <atomic>
#include <thread>
#include <future>
#include <mutex>
#endif
#if MT_HAS_17
#include <shared_mutex>
#endif
#if MT_HAS_20
#include <barrier>
#include <coroutine>
#include <latch>
#include <semaphore>
#include <stop_token>
#endif

namespace {

void print_map() {
    std::cout <<
        "concurrency / async map  (docs/multy_thread.md chapter 1)\n"
        "  01 hardware        cache line, MESI, store buffer, false sharing\n"
        "  02 ISA/asm         CAS, LL-SC, xchg, fence, acquire/release\n"
        "  03 memory model    atomicity, visibility, ordering, happens-before\n"
        "  04 OS              process, kernel thread, scheduler, ctx switch\n"
        "  05 sync primitives mutex, rwlock, semaphore, condvar, latch, barrier\n"
        "  06 correctness     data race, lost update, deadlock, ABA, TOCTOU\n"
        "  07 atomic/lockfree CAS, RMW, lock-free, wait-free, reclamation\n"
        "  08 task/future     future, promise, async, packaged_task\n"
        "  09 coroutine       suspend, continuation, cancellation, scope\n"
        "  10 async I/O       select/poll/epoll/kqueue/IOCP/io_uring, event loop\n"
        "  11 stream/channel  queue, channel, flow, backpressure\n"
        "  12 distributed     logical clock, OCC/version, lease lock, consensus\n";
}

void print_index() {
    // 层号 -> 文件：四条主线（A 底层 / B 传统 / C 异步 / D 数据流）都能对上
    std::cout << "\nlayer -> demo file\n";
    std::cout << "  01/23  mt_06_false_sharing.cpp        (hardware + perf tree)\n";
    std::cout << "  02     mt_05_lockfree.cpp             (CAS before std::atomic)\n";
    std::cout << "  03     mt_01_lost_update.cpp, mt_02_memory_orders.cpp\n";
    std::cout << "  04     mt_09_thread_pool.cpp          (thread -> pool)\n";
    std::cout << "  05     mt_03_sync_primitives.cpp, mt_08_latch_barrier_semaphore.cpp\n";
    std::cout << "  06     mt_04_correctness.cpp\n";
    std::cout << "  07     mt_05_lockfree.cpp\n";
    std::cout << "  08     mt_07_future_promise.cpp\n";
    std::cout << "  09     mt_10_coroutine.cpp\n";
    std::cout << "  10     mt_11_event_loop.cpp\n";
    std::cout << "  11     mt_12_channel_backpressure.cpp\n";
    std::cout << "  12     mt_13_distributed_sketch.cpp\n";
}

void print_capabilities() {
    std::cout << "\ncapability matrix at current level (" << mt_std_name() << ")\n";

    std::cout << "  language  : C++98=" << (MT_HAS_11 ? "ok" : "ok")
              << "  C++11=" << (MT_HAS_11 ? "yes" : "no")
              << "  C++14=" << (MT_HAS_14 ? "yes" : "no")
              << "  C++17=" << (MT_HAS_17 ? "yes" : "no")
              << "  C++20=" << (MT_HAS_20 ? "yes" : "no")
              << "  C++23=" << (MT_HAS_23 ? "yes" : "no")
              << "  C++26=" << (MT_HAS_26 ? "yes" : "no") << "\n";

    // OS 层能力不依赖语言级别：C++98 也照样有线程和 CAS，这是本系列的关键前提
    std::cout << "  os thread : available (CreateThread / pthread_create)\n";
    std::cout << "  os atomic : " << MT_OS_ATOMIC_NAME << "\n";
    std::cout << "  cache line: " << (long long)mt::cache_line() << " bytes\n";

#if MT_HAS_11
    std::cout << "  std::thread / std::atomic : available\n";
#if MT_HAS_17
    // is_always_lock_free 是 C++17 才有的静态成员
    std::cout << "  atomic<int>::is_always_lock_free = "
              << (std::atomic<int>::is_always_lock_free ? "true" : "false") << "\n";
#else
    std::atomic<int> probe(0);
    std::cout << "  atomic<int>::is_lock_free() = "
              << (probe.is_lock_free() ? "true" : "false")
              << "  (C++11 只能问实例，C++17 才有 is_always_lock_free)\n";
#endif
#else
    mt_skip(11, "std::thread / std::atomic",
            "C++11 才把线程和原子写进标准库，C++98/03 只能走 OS API 或编译器内置");
#endif

#if MT_HAS_17
    std::cout << "  std::shared_mutex / scoped_lock : available\n";
#else
    mt_skip(17, "std::shared_mutex, std::scoped_lock",
            "C++17 才有 shared_mutex 与多锁一次到位的 scoped_lock");
#endif

#if MT_HAS_20 && defined(__cpp_lib_latch)
    std::cout << "  std::latch / std::barrier / std::counting_semaphore : available\n";
#else
    mt_skip(20, "std::latch, std::barrier, std::counting_semaphore",
            "C++20 (P0431/P0883) 才进标准库；低标准下 mt_08 用 mutex+condvar 手写同款语义");
#endif

#if MT_HAS_20 && defined(__cpp_lib_jthread)
    std::cout << "  std::jthread / stop_token : available\n";
#else
    mt_skip(20, "std::jthread, std::stop_token",
            "C++20 才有可自动 join 且能协作式取消的 jthread");
#endif

#if MT_HAS_20 && defined(__cpp_lib_atomic_wait)
    std::cout << "  std::atomic::wait / notify : available\n";
#else
    mt_skip(20, "std::atomic::wait/notify",
            "C++20 的 futex 式原子等待；mt_05 会退化成 spin");
#endif

#if MT_HAS_20 && defined(__cpp_impl_coroutine)
    std::cout << "  coroutine (co_await/co_yield) : available\n";
#else
    mt_skip(20, "std::coroutine",
            "C++20 才有语言级协程；mt_10 会退回手写状态机版本");
#endif
}

}  // namespace

int main() {
    std::cout << "mt_00_map : knowledge map + capability probe\n";
    print_map();
    print_index();
    print_capabilities();

    std::cout << "\nfour main lines (chapter 24)\n"
              << "  A  CPU -> asm -> memory model -> atomic   : mt_01 mt_02 mt_05 mt_06\n"
              << "  B  OS  -> thread -> lock -> scheduler     : mt_03 mt_04 mt_08 mt_09\n"
              << "  C  I/O -> event loop -> future -> coroutine: mt_07 mt_10 mt_11\n"
              << "  D  queue -> channel -> stream -> reactive   : mt_12 mt_13\n";

    std::cout << "\nread docs/multy_thread.md for the reasoning behind each layer.\n";
    return 0;
}
