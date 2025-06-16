#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

// === std::mutex 示例 ===
std::mutex mtx;

void print_from_thread(int id) {
  mtx.lock();
  cout << "Thread " << id << " is executing." << endl;
  this_thread::sleep_for(seconds(1));
  cout << "Thread " << id << " is done." << endl;
  mtx.unlock();
}

// === std::timed_mutex 示例 ===
std::timed_mutex tmtx;

void timed_mutex_demo(int id) {
  auto now = system_clock::now();
  if (tmtx.try_lock_until(now + seconds(2))) {
    cout << "Thread " << id << " acquired timed_mutex." << endl;
    this_thread::sleep_for(seconds(1));
    tmtx.unlock();
  } else {
    cout << "Thread " << id << " timed out waiting for timed_mutex." << endl;
  }
}

// === std::recursive_mutex 示例 ===
std::recursive_mutex rmtx;

void recursive_func(int level) {
  rmtx.lock();
  cout << "Recursive level " << level << " entered." << endl;
  if (level > 0)
    recursive_func(level - 1);
  rmtx.unlock();
}

void recursive_mutex_demo() {
  recursive_func(2);  // 同一线程递归加锁
}

int main() {
  cout << "=== Part 1: std::mutex demo ===" << endl;
  {
    thread t1(print_from_thread, 1);
    thread t2(print_from_thread, 2);
    t1.join();
    t2.join();
  }

  cout << "\n=== Part 2: std::timed_mutex demo ===" << endl;
  {
    thread t1(timed_mutex_demo, 1);
    thread t2(timed_mutex_demo, 2);
    t1.join();
    t2.join();
  }

  cout << "\n=== Part 3: std::recursive_mutex demo ===" << endl;
  {
    thread t(recursive_mutex_demo);
    t.join();
  }

  cout << "\nMain done." << endl;
  return 0;
}