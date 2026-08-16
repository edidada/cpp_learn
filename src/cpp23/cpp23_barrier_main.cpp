#include <iostream>
#include <thread>
#include <vector>
#include <barrier>

void worker(std::barrier<>& sync_point, int id) {
    std::cout << "Thread " << id << " is doing some work..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread " << id << " reached the barrier." << std::endl;

    sync_point.arrive_and_wait();
    std::cout << "Thread " << id << " is continuing after the barrier." << std::endl;
}

int main() {
    constexpr size_t num_threads = 5;
    std::barrier<> sync_point(num_threads);

    std::vector<std::thread> threads;
    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, std::ref(sync_point), static_cast<int>(i + 1));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads have passed the barrier." << std::endl;
}
