#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

queue<int> data_queue;
mutex mtx;
condition_variable cv_producer, cv_consumer;

bool done = false;

void producer(int id, int count) {
  for (int i = 0; i < count; ++i) {
    unique_lock<mutex> lock(mtx);
    cv_producer.wait(lock, []{ return data_queue.empty(); });

    cout << "Producer " << id << " producing data: " << i << endl;
    data_queue.push(i);

    lock.unlock();
    cv_consumer.notify_one();

    this_thread::sleep_for(chrono::milliseconds(300));
  }

  // 计划在当前线程完全结束时调用 notify_all
  notify_all_at_thread_exit(cv_consumer, unique_lock<mutex>(mtx));
}

void consumer(int count) {
  for (int i = 0; i < count; ++i) {
    unique_lock<mutex> lock(mtx);

    auto now = chrono::system_clock::now();
    bool has_data = cv_consumer.wait_until(lock, now + chrono::seconds(1), []{
      return !data_queue.empty();
    });

    if (!has_data) {
      cout << "[Consumer] Wait timeout." << endl;
      continue;
    }

    int value = data_queue.front();
    data_queue.pop();
    cout << "Consumer consuming data: " << value << endl;

    lock.unlock();
    cv_producer.notify_one();
  }
}

int main() {
  int prod_count = 5;
  thread prod(producer, 1, prod_count);

  for (int i = 0; i < prod_count; ++i) {
    consumer(i);
  }

  if (prod.joinable()) prod.join();

  cout << "Main thread finished." << endl;
  return 0;
}