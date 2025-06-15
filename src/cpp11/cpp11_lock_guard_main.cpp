#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

// === 示例 1: recursive_timed_mutex ===
std::recursive_timed_mutex rmtx;

void recursive_func(int level) {
  // 使用 try_lock_for 尝试获取锁，最多等待 100ms
  if (rmtx.try_lock_for(milliseconds(100))) {
    cout << "Thread " << this_thread::get_id()
         << ": Recursive level " << level << endl;
    if (level > 0)
      recursive_func(level - 1);
    rmtx.unlock();
  } else {
    cout << "Thread " << this_thread::get_id()
         << ": Timeout in recursive_func at level " << level << endl;
  }
}

// === 示例 2: unique_lock ===
std::mutex mtx;
int shared_data = 0;

void modify_data() {
  std::unique_lock<std::mutex> lock(mtx);  // 自动加锁解锁
  shared_data++;
  cout << "Thread " << this_thread::get_id()
       << " modified data to " << shared_data << endl;
  lock.unlock();  // 可手动提前释放锁
  this_thread::sleep_for(milliseconds(100));  // 模拟其他操作
}

// === 示例 3: call_once / once_flag ===
std::once_flag init_flag;

void initialize_once() {
  std::call_once(init_flag, []{
    cout << "Initializing resource once by thread "
         << this_thread::get_id() << endl;
  });
}

// === 示例 4: lock_guard 替代 scoped_lock (C++17 特性) ===
// 在 C++11 中可以使用 lock_guard，并发安全地锁定多个锁

void safe_lock_two() {
  std::lock_guard<std::mutex> l1(mtx);
  std::lock_guard<std::mutex> l2(mtx);  // 同一个 mutex 可以重复 lock（如果是 recursive_mutex）
  cout << "Locked two times safely." << endl;
}

int main() {
  cout << "=== Part 1: recursive_timed_mutex demo ===" << endl;
  {
    thread t1(recursive_func, 2);
    thread t2(recursive_func, 2);
    t1.join();
    t2.join();
  }

  cout << "\n=== Part 2: unique_lock demo ===" << endl;
  {
    thread t1(modify_data);
    thread t2(modify_data);
    t1.join();
    t2.join();
  }

  cout << "\n=== Part 3: call_once / once_flag demo ===" << endl;
  {
    thread t1(initialize_once);
    thread t2(initialize_once);
    thread t3(initialize_once);
    t1.join();
    t2.join();
    t3.join();
  }

  cout << "\n=== Part 4: lock_guard as scoped_lock replacement ===" << endl;
  {
    thread t(safe_lock_two);
    t.join();
  }

  cout << "\nMain done." << endl;
  return 0;
}