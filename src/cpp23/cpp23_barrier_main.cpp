#include <iostream>
#include <thread>
#include <vector>

// CI 上 macos job 报的是 "no member named 'ref' in namespace 'std'"：
// std::ref 在 <functional>，std::chrono::milliseconds 在 <chrono>，
// 这里原来都靠别的头传递包含侥幸带进来，libstdc++ 给了、libc++ 不一定给。头写全。
#include <chrono>
#include <cstddef>
#include <functional>

// std::barrier 是 C++20 库设施（__cpp_lib_barrier），老 libstdc++/libc++ 与
// 某些 win32-thread 模型的 MinGW 上要么没有、要么不可用；探到才编正文，
// 否则只打 [SKIP]，别让一个特性把整个 target 拖红。
#if defined(__has_include)
#  if __has_include(<barrier>)
#    include <barrier>
#  endif
#endif

#if defined(__cpp_lib_barrier)

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

#else

int main() {
    std::cout << "[SKIP] std::barrier 不可用（需 C++20 且标准库实现了 <barrier>，"
                 "本条编译环境没有）\n";
}

#endif
