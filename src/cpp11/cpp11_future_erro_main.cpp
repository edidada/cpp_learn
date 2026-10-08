#include <iostream>
#include <future>
#include <thread>
#include <stdexcept>      // std::runtime_error
#include <string>

// 异步线程函数：模拟一个可能抛出异常的任务
void task_promise(std::promise<int>& p) {
  try {
    std::cout << "Task is running..." << std::endl;
    throw std::runtime_error("Something went wrong in the task!");  // 抛出异常
  } catch (...) {
    // 捕获异常并传递给 promise
    p.set_exception(std::current_exception());
  }
}

// 示例：展示如何捕获 future 错误码
void check_future_error() {
  std::promise<void> p;
  std::future<void> f = p.get_future();

  try {
    // 尝试获取尚未设置值的 promise 的 future 值
    f.get();
  } catch (const std::future_error& e) {
    std::cerr << "Caught a future_error with code: " << e.code() << std::endl;
    std::cerr << "Error message: " << e.what() << std::endl;
  }
}

int main() {
  std::cout << "=== Part 1: 使用 set_exception 传递异常 ===" << std::endl;
  {
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t(task_promise, std::ref(p));
    try {
      int result = f.get();  // 获取异常
    } catch (const std::exception& e) {
      std::cerr << "Exception caught from future: " << e.what() << std::endl;
    }

    t.join();
  }

  std::cout << "\n=== Part 2: 捕获 future_error ===" << std::endl;
  {
    check_future_error();
  }

  std::cout << "\n=== Part 3: std::uses_allocator<std::promise> ===" << std::endl;
  {
    // 虽然 std::promise 支持 allocator，但几乎不用
    std::allocator<int> alloc;

    // 实际上 promise 内部数据的分配器是 void 类型
    using promise_type = std::promise<int>;
    static_assert(std::uses_allocator<promise_type, std::allocator<int>>::value,
                  "std::promise should support allocator");

    std::cout << "std::uses_allocator<std::promise<int>, std::allocator<int>> is true." << std::endl;
  }

  return 0;
}