#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 模拟耗时任务
int long_task() {
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Task completed in thread ID: " << std::this_thread::get_id() << std::endl;
  return 123;
}

int main() {
  std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

  // 使用 async 策略启动异步任务
  std::future<int> f1 = std::async(std::launch::async, long_task);
  std::cout << "Async task started..." << std::endl;

  // 使用 deferred 策略延迟执行任务
  std::future<int> f2 = std::async(std::launch::deferred, long_task);
  std::cout << "Deferred task created, will run when get() is called." << std::endl;

  // 获取 async 结果
  std::cout << "Waiting for async result..." << std::endl;
  int result1 = f1.get();  // 等待异步线程完成
  std::cout << "Async result: " << result1 << std::endl;

  // 获取 deferred 结果（此时任务才开始执行）
  std::cout << "Getting deferred result..." << std::endl;
  int result2 = f2.get();  // 此刻在主线程执行
  std::cout << "Deferred result: " << result2 << std::endl;

  std::cout << "Main done." << std::endl;
  return 0;
}