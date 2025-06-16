#include <iostream>
#include <thread>
#include <future>     // std::packaged_task, std::future
#include <chrono>    // std::chrono::seconds

using namespace std;

// 模拟一个耗时的计算任务
int compute_square(int x) {
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return x * x;
}

void run_task(std::packaged_task<int()> &task) {
  std::cout << "Task is running in a separate thread..." << std::endl;
  task();  // 执行任务，触发 future 的就绪状态
}

int main() {
  // 1. 创建 packaged_task，封装任务函数 compute_square
  std::packaged_task<int()> task([]{ return compute_square(7); });

  // 2. 获取与 task 关联的 future，用于获取任务返回值
  std::future<int> result = task.get_future();

  // 3. 在新线程中运行任务
  std::thread worker(run_task, std::ref(task));

  // 4. 主线程等待结果
  std::cout << "Main thread waiting for result..." << std::endl;
  int value = result.get();  // 阻塞直到任务完成

  // 5. 输出结果
  std::cout << "Result from task: " << value << std::endl;

  // 6. 等待线程结束
  worker.join();

  std::cout << "Main done." << std::endl;
  return 0;
}