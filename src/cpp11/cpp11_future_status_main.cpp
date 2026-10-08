#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 模拟耗时任务
int long_task() {
  this_thread::sleep_for(seconds(3));  // C++11 合法写法
  return 42;
}

int main() {
  // 启动异步任务
  future<int> f = async(launch::async, long_task);

  cout << "Checking future status..." << endl;

  for (int i = 0; i < 6; ++i) {
    // 等待 500ms，检查状态
    future_status status = f.wait_for(milliseconds(500));

    if (status == future_status::ready) {
      cout << "Result is ready!" << endl;
      int result = f.get();
      cout << "Got result: " << result << endl;
      break;
    }
    else if (status == future_status::timeout) {
      cout << "Timeout, still waiting..." << endl;
    }
    else if (status == future_status::deferred) {
      cout << "Task is deferred, not applicable here." << endl;
      // 如果是 deferred 状态，可以用 f.get() 直接调用
      break;
    }

    // 模拟其他工作
    cout << "Doing some other work in main thread..." << endl;
  }

  cout << "Main done." << endl;
  return 0;
}