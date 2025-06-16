#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

using namespace std;

// 自定义锁类，满足 Lockable 概念
class MyLock {
public:
  explicit MyLock(std::mutex& m) : mtx_(m) {
    mtx_.lock();
  }

  ~MyLock() {
    mtx_.unlock();
  }

  void lock() {
    mtx_.lock();
  }

  void unlock() {
    mtx_.unlock();
  }

private:
  std::mutex& mtx_;
};

// 全局变量
std::mutex g_mutex;
std::condition_variable_any g_cv;
bool g_ready = false;

void wait_for_data() {
  MyLock lock(g_mutex);
  cout << "Waiting for data..." << endl;
  g_cv.wait(lock, []{ return g_ready; });
  cout << "Data is ready!" << endl;
}

void set_data_ready() {
  std::this_thread::sleep_for(std::chrono::seconds(2));  // C++11 合法
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_ready = true;
    cout << "Notifying all..." << endl;
  }
  g_cv.notify_all();
}

int main() {
  std::thread t1(wait_for_data);
  std::thread t2(set_data_ready);

  t1.join();
  t2.join();

  cout << "Main done." << endl;
  return 0;
}