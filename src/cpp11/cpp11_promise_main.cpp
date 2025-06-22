#include <iostream>
#include <thread>
#include <future>   // std::promise, std::future
#include <chrono>  // std::chrono::seconds

using namespace std;

// 线程函数：设置 promise 的值
void produce_data(std::promise<int>& px) {
  std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟耗时操作

  int result = 42;  // 模拟计算结果
  std::cout << "Producer: Setting the value..." << std::endl;
  px.set_value(result);  // 通过 promise 设置值
}

// 线程函数：等待并获取 future 的值
void consume_data(std::future<int>& fx) {
  std::cout << "Consumer: Waiting for the value..." << std::endl;

  int value = fx.get();  // 阻塞直到 promise 设置了值
  std::cout << "Consumer: Received value = " << value << std::endl;
}

int main() {
  std::promise<int> p;
  std::future<int> f = p.get_future();  // 与 promise 关联的 future

  std::thread producer(produce_data, std::ref(p));
  std::thread consumer(consume_data, std::ref(f));

  std::cout << "Main thread waiting for threads to finish..." << std::endl;

  producer.join();
  consumer.join();

  std::cout << "Main done." << std::endl;
  return 0;
}