#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void wait_for_data() {
  std::unique_lock<std::mutex> lock(mtx);

  cout << "Waiting for data..." << endl;

  // 不带谓词的 wait_for，返回值是 cv_status
  std::cv_status status = cv.wait_for(lock, std::chrono::seconds(3));

  if (status == std::cv_status::timeout) {
    cout << "Timeout! No data received." << endl;
  } else {
    // 醒来后手动检查条件
    if (ready) {
      cout << "Data is ready!" << endl;
    } else {
      cout << "Spurious wake-up!" << endl;
    }
  }
}

void provide_data() {
  std::this_thread::sleep_for(std::chrono::seconds(5));  // 数据延迟提供
  std::unique_lock<std::mutex> lock(mtx);
  ready = true;
  cv.notify_one();
}

int main() {
  std::thread t1(wait_for_data);
  std::thread t2(provide_data);

  t1.join();
  t2.join();

  cout << "Main thread finished." << endl;
  return 0;
}