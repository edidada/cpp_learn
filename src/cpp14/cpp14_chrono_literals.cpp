#include <iostream>
#include <chrono>
#include <thread>

void test_chrono_literals() {
    std::cout << "=== Chrono Literals ===" << std::endl;
    
    using namespace std::chrono_literals;
    
    auto h = 2h;
    auto min = 30min;
    auto s = 45s;
    auto ms = 100ms;
    auto us = 200us;
    auto ns = 300ns;
    
    std::cout << "2 hours = " << h.count() << " hours" << std::endl;
    std::cout << "30 min = " << min.count() << " minutes" << std::endl;
    std::cout << "45 s = " << s.count() << " seconds" << std::endl;
    std::cout << "100 ms = " << ms.count() << " milliseconds" << std::endl;
    std::cout << "200 us = " << us.count() << " microseconds" << std::endl;
    std::cout << "300 ns = " << ns.count() << " nanoseconds" << std::endl;
}

void test_duration_operations() {
    std::cout << "\n=== Duration Operations ===" << std::endl;
    
    using namespace std::chrono_literals;
    
    auto total = 1h + 30min + 45s;
    std::cout << "Total seconds: " 
              << std::chrono::duration_cast<std::chrono::seconds>(total).count() 
              << std::endl;
    
    auto half = 30min / 2;
    std::cout << "30min / 2 = " << half.count() << " minutes" << std::endl;
    
    auto doubled = 15s * 2;
    std::cout << "15s * 2 = " << doubled.count() << " seconds" << std::endl;
}

void test_sleep_with_literals() {
    std::cout << "\n=== Sleep with Literals ===" << std::endl;
    
    using namespace std::chrono_literals;
    
    auto start = std::chrono::steady_clock::now();
    
    std::cout << "Sleeping for 100ms..." << std::endl;
    std::this_thread::sleep_for(100ms);
    
    auto end = std::chrono::steady_clock::now();
    auto actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Actually slept for " << actual.count() << " milliseconds" << std::endl;
}

int main() {
    test_chrono_literals();
    test_duration_operations();
    test_sleep_with_literals();
    return 0;
}
