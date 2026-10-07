// mt_07_future_promise.cpp —— Task / Future：表示"还没算出来的结果"
//
// 对应 docs/multy_thread.md 第九节（第八层）：
//   Thread -> 执行函数 -> join -> 获得结果      （旧世界：结果和线程绑死）
//   Task -> Future<Result>                      （新世界：结果是一等对象，能传递能组合）
//   C++ future / Java Future / CompletableFuture / C# Task / JS Promise / Rust Future
//
// 顺序刻意反着讲：先在 C++98 上手搓一个 future，理解它到底是什么，
// 再上 std::future，看标准库替我们多做了什么（异常传递、状态机、共享、移动语义）。
#include "mt_common.h"

#if MT_HAS_11
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#endif

namespace {

// ---------------------------------------------------------------------------
// 1) 手搓 future（C++98 能跑）：一个值 + 一个就绪位 + 一个等待循环
// ---------------------------------------------------------------------------
struct Result98 {
    Result98() : ready(0), value(0) {}
    volatile int ready;
    long value;
};

void slow_compute(void* p) {
    Result98* r = static_cast<Result98*>(p);
    mt::sleep_ms(30);                 // 假装在算很贵的东西
    r->value = 6 * 7;
    mt::os_fence();                   // release：值先可见，就绪位后置
    r->ready = 1;
}

long wait_result98(Result98* r) {
    while (!r->ready) { mt::sleep_ms(1); }   // 生产上这里该用 condvar/futex，别空转
    mt::os_fence();                          // acquire
    return r->value;
}

#if MT_HAS_11
// ---------------------------------------------------------------------------
// 2) std::promise / std::future：把"谁负责产生值"和"谁负责取值"解耦
// ---------------------------------------------------------------------------
void produce(std::promise<long> p) {         // promise 按值移动进来：所有权转移
    try {
        mt::sleep_ms(20);
        p.set_value(42);
    } catch (...) {
        p.set_exception(std::current_exception());
    }
}

// 故意抛异常，看它怎么穿过线程边界在调用方重新出现
void produce_throws(std::promise<long> p) {
    p.set_exception(std::make_exception_ptr(std::runtime_error("boom from worker")));
}

// packaged_task：把可调用对象包装成"任务"，交给线程池调度（层 08 与主线 B 的连接点）

#endif  // MT_HAS_11

}  // namespace

int main() {
    std::cout << "mt_07_future_promise : a value that does not exist yet\n";

    // ---- 1. 手搓 ----
    mt_section(8, "hand-rolled future, so you can see what the abstraction is made of");
    {
        Result98 r;
        mt::Thread t;
        t.start(&slow_compute, &r);
        std::cout << "  main thread can keep doing work while waiting\n";
        std::cout << "  result = " << wait_result98(&r) << " (expect 42)\n";
        t.join();
    }
    std::cout << "  本质就三件事：存放值的盒子、就绪状态、等待方式。\n";
    std::cout << "  标准库额外解决的：所有权转移、异常传播、多消费者、超时、取消。\n";

#if MT_HAS_11
    // ---- 2. std::async ----
    mt_section(8, "std::async: 最省事的 task，不用自己管线程");
    {
        auto f = std::async(std::launch::async, [](int a) { return a + 1; }, 10);
        std::cout << "  async result = " << f.get() << "\n";
        // launch::deferred：不立即起线程，直到 get()/wait() 才惰性执行（同一条线里跑）
        auto d = std::async(std::launch::deferred, [](int a) { return a * 10; }, 5);
        std::cout << "  status before get (deferred) = "
                  << (d.wait_for(std::chrono::seconds(0)) == std::future_status::ready
                          ? "ready" : "deferred/not-ready") << "\n";
        std::cout << "  deferred result = " << d.get() << "\n";
    }

    // ---- 3. promise/future 显式配对 ----
    mt_section(8, "promise + future: 生产端与消费端各自掌握一半");
    {
        std::promise<long> p;
        std::future<long>  f = p.get_future();
        std::thread t(&produce, std::move(p));
        std::cout << "  future.get() = " << f.get() << "\n";
        t.join();
    }

    // ---- 4. 异常跨线程 ----
    mt_section(8, "exception travels through the future, not through the stack");
    {
        std::promise<long> p;
        std::future<long>  f = p.get_future();
        std::thread t(&produce_throws, std::move(p));
        try {
            long v = f.get();
            std::cout << "  unexpected value " << v << "\n";
        } catch (const std::exception& e) {
            std::cout << "  caught in main thread: " << e.what() << "\n";
        }
        t.join();
    }

    // ---- 5. packaged_task ----
    mt_section(8, "packaged_task: 可调用对象 + 结果通道 = 可投递的任务");
    {
        std::packaged_task<long(int)> task([](int x) { return x * x; });
        std::future<long> f = task.get_future();
        std::thread t(std::move(task), 9);
        std::cout << "  packaged_task result = " << f.get() << " (expect 81)\n";
        t.join();
    }

    // ---- 6. shared_future：一对多 ----
    mt_section(8, "shared_future: 多个消费者读同一个结果");
    {
        std::promise<long> p;
        std::shared_future<long> sf = p.get_future().share();
        std::vector<std::thread> v;
        for (int i = 0; i < 3; ++i) {
            v.push_back(std::thread([sf, i] {
                long x = sf.get();
                std::cout << "  consumer " << i << " got " << x << "\n";
            }));
        }
        p.set_value(7);
        for (int i = 0; i < (int)v.size(); ++i) v[i].join();
    }

    // ---- 7. 组合 ----
    mt_section(8, "composition: 这才是 CompletableFuture / Promise / Task 真正的价值");
    {
        std::future<long> base = std::async(std::launch::async, []() -> long { return 5; });
        // C++ 标准库到 C++23 都没有 future.then()。可移植做法：shared_ptr 持有上游 future，
        // 再把它交给下一个任务。（C++20 起可以直接 [f = std::move(base)] 捕获）
        std::shared_ptr<std::future<long>> upstream(
            new std::future<long>(std::move(base)));
        std::future<long> chained = std::async(std::launch::async,
            [upstream] { return upstream->get() + 1000; });
        std::cout << "  chained result = " << chained.get() << " (expect 1005)\n";
    }
    std::cout << "  真实项目里这一层交给协程：co_await 就是 then() 的语法糖，见 mt_10。\n";
#else
    mt_skip(11, "std::future / std::promise / std::async / packaged_task",
            "C++11 才有；上面的手搓版说明缺的只是语法糖，概念在 C++98 一样能落地");
#endif

    std::cout << "\nmap: 一个未来值 -> JS Promise / Java Future / C# Task<T> / C++ std::future\n"
                 "     挂起一个计算 -> suspend / async-await / coroutine\n"
                 "     多个异步值   -> Flow / Flux / AsyncIterator（见 mt_12）\n";
    return 0;
}
