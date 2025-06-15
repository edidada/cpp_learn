#include <iostream>
#include <thread>
#include <future>     // std::future, std::shared_future
#include <chrono>    // std::chrono::seconds
#include <vector>

using namespace std;

// 模拟一个耗时的计算任务
int compute_value() {
  std::this_thread::sleep_for(std::chrono::seconds(3));
  return 42;
}

// 多个消费者线程等待同一个 shared_future
void consumer(int id, const std::shared_future<int>& result) {
  std::cout << "Consumer " << id << " waiting for result..." << std::endl;
  int value = result.get();  // 所有线程在此阻塞直到结果就绪
  std::cout << "Consumer " << id << " received value: " << value << std::endl;
}

int main() {
  std::cout << "Main thread starting async task..." << std::endl;

  // 启动异步任务：返回 future
  std::future<int> f = std::async(std::launch::async, compute_value);

  // 将 future 转换为 shared_future，允许多个线程访问
  std::shared_future<int> sf = f.share();

  // 创建多个消费者线程
  const int num_consumers = 5;
  std::vector<std::thread> consumers;

  for (int i = 0; i < num_consumers; ++i) {
    consumers.emplace_back(consumer, i + 1, std::ref(sf));
  }

  std::cout << "Main thread waiting for threads to finish..." << std::endl;

  for (auto& t : consumers) {
    if (t.joinable()) {
      t.join();
    }
  }

  std::cout << "Main done." << std::endl;
  return 0;
}