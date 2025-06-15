#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

// === 示例 1: std::lock 和 std::try_lock ===
std::mutex m1, m2;

void try_lock_demo() {
  int result = try_lock(m1, m2);
  if (result == -1) {
    cout << "Thread " << this_thread::get_id()
         << ": Successfully locked both mutexes!" << endl;
    m1.unlock();
    m2.unlock();
  } else {
    cout << "Thread " << this_thread::get_id()
         << ": Failed to lock mutex[" << result << "]" << endl;
  }
}

void lock_demo() {
  lock(m1, m2);  // 自动按安全顺序加锁，避免死锁
  cout << "Thread " << this_thread::get_id()
       << ": Locked both mutexes with std::lock." << endl;
  m1.unlock();
  m2.unlock();
}

// === 示例 2: std::call_once ===
std::once_flag init_flag;

void initialize_resource() {
  call_once(init_flag, []{
    cout << "Initializing resource once by thread "
         << this_thread::get_id() << endl;
  });
}

// === 示例 3: std::swap(std::unique_lock) ===
std::mutex mtx;

void swap_unique_locks() {
  std::unique_lock<std::mutex> lock1(mtx, std::defer_lock);
  std::unique_lock<std::mutex> lock2(mtx, std::defer_lock);

  lock1.lock();  // lock1 拥有锁
  cout << "Before swap: lock1 owns lock? " << lock1.owns_lock() << endl;
  cout << "Before swap: lock2 owns lock? " << lock2.owns_lock() << endl;

  std::swap(lock1, lock2);  // 交换锁的状态

  cout << "After swap: lock1 owns lock? " << lock1.owns_lock() << endl;
  cout << "After swap: lock2 owns lock? " << lock2.owns_lock() << endl;
}

int main() {
  cout << "=== Part 1: std::try_lock demo ===" << endl;
  {
    thread t1(try_lock_demo);
    thread t2(try_lock_demo);
    t1.join();
    t2.join();
  }

  cout << "\n=== Part 2: std::lock demo ===" << endl;
  {
    thread t1(lock_demo);
    thread t2(lock_demo);
    t1.join();
    t2.join();
  }

  cout << "\n=== Part 3: std::call_once demo ===" << endl;
  {
    thread t1(initialize_resource);
    thread t2(initialize_resource);
    thread t3(initialize_resource);
    t1.join();
    t2.join();
    t3.join();
  }

  cout << "\n=== Part 4: std::swap(std::unique_lock) demo ===" << endl;
  {
    swap_unique_locks();
  }

  cout << "\nMain done." << endl;
  return 0;
}