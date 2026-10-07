// mt_10_coroutine.cpp —— 协程：从手写状态机到 co_await
//
// 对应 docs/multy_thread.md：
//   十、第九层 Coroutine（suspend / continuation / cancellation / scope）
//   二十一节表格 ASM 列写的就是"手工状态机"，第一版就是这个
//   C++20 coroutine 更像"底层机制"，标准库没给完整 runtime（文档特意强调过）
//
// 关键分界线（文档第六节那段）：
//   线程 mutex  -> wait  -> 可能阻塞整个线程
//   协程 Mutex  -> suspend -> 只挂起这个协程，线程可以去做别的协程
// 所以第一段先在没有协程语言支持时手工做一个，第二段再看 C++20 版本。
#include "mt_common.h"

#if MT_HAS_20 && defined(__cpp_impl_coroutine)
#include <coroutine>
#include <future>
#include <thread>
#include <chrono>
#endif

// ---------------------------------------------------------------------------
// 1) protothread：用 switch 落点实现"从上次 yield 处继续"
//
// 原理就是编译器给 co_await 做的事：
//   - 局部状态从调用栈搬到一个显式的结构体（协程帧）
//   - yield = 记录当前行号 + return
//   - resume = switch 跳到那个行号继续执行
// 没有任何库/OS 支持，纯语言特性就够 —— 所以 C++98 也能跑。
//
// 重要陷阱：yield 之后函数是 return 出去的，所有局部变量都会重新初始化！
// 因此需要跨挂起点存活的状态，必须放进 CoState（那就是协程帧的雏形）。
// ---------------------------------------------------------------------------
struct CoState { int pos; int i; };

#define CO_BEGIN(s)      switch ((s)->pos) { case 0:
#define CO_END(s)        } (s)->pos = -1;
#define CO_YIELD(s)      do { (s)->pos = __LINE__; return; case __LINE__: ; } while (0)

CoState g_c1, g_c2, g_c3;
long g_ticks = 0;

void count_thread(CoState* self, const char* name, int n) {
    CO_BEGIN(self);
    for (self->i = 1; self->i <= n; ++self->i) {
        std::cout << "    " << name << " yields " << self->i << "\n";
        CO_YIELD(self);              // 让出：不是阻塞线程，是回到调度器
    }
    std::cout << "    " << name << " finished\n";
    CO_END(self);
}

void scheduler_run() {
    CoState* all[3] = { &g_c1, &g_c2, &g_c3 };
    const char* names[3] = { "A", "B", "C" };
    for (int i = 0; i < 3; ++i) { all[i]->pos = 0; all[i]->i = 0; }
    bool alive = true;
    while (alive) {
        alive = false;
        for (int i = 0; i < 3; ++i) {
            if (all[i]->pos >= 0) {
                alive = true;
                ++g_ticks;
                count_thread(all[i], names[i], 3);
            }
        }
    }
}

#if MT_HAS_20 && defined(__cpp_impl_coroutine)
// ---------------------------------------------------------------------------
// 2) C++20 generator：co_yield 版
// ---------------------------------------------------------------------------
template <typename T>
class Generator {
public:
    struct promise_type {
        T current;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) { current = v; return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    explicit Generator(std::coroutine_handle<promise_type> h) : h_(h) {}
    ~Generator() { if (h_) h_.destroy(); }
    Generator(Generator&& o) noexcept : h_(o.h_) { o.h_ = nullptr; }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    bool next() { if (h_.done()) return false; h_.resume(); return !h_.done(); }
    const T& value() const { return h_.promise().current; }

private:
    std::coroutine_handle<promise_type> h_;
};

Generator<int> upto(int n) {
    for (int i = 1; i <= n; ++i) co_yield i;    // 状态机由编译器生成
}

Generator<long> fibs(int n) {
    long a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield b;
        long t = a + b; a = b; b = t;
    }
}

// ---------------------------------------------------------------------------
// 3) co_await 一个"挂起而不是阻塞"的 awaitable：
//    await_suspend 里把 continuation 交给另一个线程，本线程立刻回到调用者。
//    —— 这正是文档里 Coroutine vs Thread 那张图的含义。
// ---------------------------------------------------------------------------
struct SleepAwaitable {
    int ms;
    bool await_ready() const noexcept { return ms <= 0; }
    void await_suspend(std::coroutine_handle<> h) const {
        int d = ms;                            // 协程帧之外，成员随时可能失效，先取值
        // 真实 runtime 这里会挂到定时器堆/epoll 回调上，而不是起线程
        std::thread([h, d] {
            mt::sleep_ms((unsigned)d);
            h.resume();                        // 换一个线程继续跑协程体
        }).detach();
    }
    void await_resume() const noexcept {}
};

struct Task {
    struct promise_type {
        std::promise<void> fin;
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept { return {}; }   // 立即开跑
        void return_void() {}
        void unhandled_exception() { fin.set_exception(std::current_exception()); }

        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept {
                h.promise().fin.set_value();       // 叫醒等待完成的人
                return std::noop_coroutine();      // 控制权交回，协程帧由 Task 负责销毁
            }
            void await_resume() noexcept {}
        };
        final_awaiter final_suspend() noexcept { return {}; }
    };

    std::coroutine_handle<promise_type> h;
    ~Task() { if (h) h.destroy(); }
};

Task sleepy(const char* tag, long* counter_while_suspended) {
    std::cout << "  [" << tag << "] before co_await, os_tid = " << mt::os_tid() << "\n";
    *counter_while_suspended = 1;                  // 调用方可以拿线程去干别的
    co_await SleepAwaitable{40};
    std::cout << "  [" << tag << "] resumed on a different thread, os_tid = "
              << mt::os_tid() << "\n";
    *counter_while_suspended = 2;
}
#endif  // C++20 coroutine

int main() {
    std::cout << "mt_10_coroutine : suspension is a state machine, nothing more\n";

    // ---- 1. 手写状态机 ----
    mt_section(9, "hand-rolled coroutine (protothread) on one thread, no library at all");
    scheduler_run();
    std::cout << "  scheduler ticks = " << g_ticks
              << "  (3 coroutines x 3 steps + final sweep)\n";
    std::cout << "  这段在 C++98 就能跑：协程 = 把'执行位置'变成数据，谁调用谁负责继续。\n";

#if MT_HAS_20 && defined(__cpp_impl_coroutine)
    // ---- 2. co_yield ----
    mt_section(9, "C++20 generator: same state machine, written by the compiler");
    {
        auto g = upto(4);
        std::cout << "  upto: ";
        while (g.next()) std::cout << g.value() << " ";
        std::cout << "\n";

        auto f = fibs(8);
        std::cout << "  fibs: ";
        while (f.next()) std::cout << f.value() << " ";
        std::cout << "\n";
    }
    std::cout << "  与第 1 段对照：局部变量活在堆上的协程帧里，而不是调用栈上。\n";

    // ---- 3. co_await + 线程交接 ----
    mt_section(9, "co_await: suspend the coroutine, keep the thread free");
    {
        long marker = 0;
        Task t = sleepy("demo", &marker);          // 跑到 co_await 就返回
        std::cout << "  caller already back, marker = " << marker << "\n"
                  << "  main thread is free here: this is the point of coroutines\n";
        t.h.promise().fin.get_future().wait();      // 等协程真正结束
        std::cout << "  after completion, marker = " << marker << " (2 = resumed later)\n";
    }

    // ---- 4. 取消 / 结构化并发 ----
    mt_section(14, "cancellation & structured concurrency（C++ 标准库到今天没给答案）");
    std::cout << "  Kotlin: CoroutineScope / Job / Dispatchers 是一整套 runtime\n"
                 "  C++   : 只有 coroutine 机制，scope/cancellation/dispatcher 要自己搭\n"
                 "  stop_token(C++20) 是最接近的现成件，见 mt_09 的 jthread 版本\n"
                 "  文档第十四节列的 orphan task / task leak / lost exception，\n"
                 "  在 C++ 里全是框架作者的责任，不是语言的责任\n";
#else
    mt_skip(20, "co_yield / co_await",
            "语言级协程是 C++20 (P0912/P2502 等)；上面 protothread 版说明了它的底层是什么");
#endif

    std::cout << "\nThread vs Coroutine:\n"
                 "  1 个 OS 线程可以承载 N 个协程 -> 挂起成本是函数返回，不是上下文切换\n"
                 "  10 万连接：thread-per-connection 要 10 万线程；协程只要几个线程\n";
    return 0;
}
