#include <coroutine>
#include <iostream>
#include <thread>

struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
    std::coroutine_handle<promise_type> handle;
};

Task example_coroutine() {
    std::cout << "Hello from coroutine!\n";
    co_return;
}

int main() {
    auto task = example_coroutine();
    task.handle.resume();

    return 0;
}