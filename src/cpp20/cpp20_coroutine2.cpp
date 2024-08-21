#include <coroutine>
#include <iostream>
#include <vector>
#include <memory>

// 协程的返回类型，用于生成整数
struct Generator {
    struct promise_type {
        int value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
        std::suspend_always yield_value(int v) {
            value = v;
            return {};
        }
    };

    std::coroutine_handle<promise_type> coro;

    Generator(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Generator() {
        if (coro) coro.destroy();
    }

    bool moveNext() { return coro.resume(), !coro.done(); }
    int currentValue() const { return coro.promise().value; }
};

// 协程函数，生成一个整数序列
Generator generateSequence() {
    for (int i = 0; i < 5; ++i) {
        co_yield i;
    }
}

int main() {
    auto seq = generateSequence();
    while (seq.moveNext()) {
        std::cout << seq.currentValue() << std::endl;
    }
    return 0;
}
