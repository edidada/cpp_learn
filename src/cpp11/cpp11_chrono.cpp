#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>

void test_duration() {
    std::cout << "=== Duration ===" << std::endl;
    
    std::chrono::hours h(2);
    std::chrono::minutes m(30);
    std::chrono::seconds s(45);
    std::chrono::milliseconds ms(100);
    std::chrono::microseconds us(200);
    std::chrono::nanoseconds ns(300);
    
    std::cout << "Hours: " << h.count() << std::endl;
    std::cout << "Minutes: " << m.count() << std::endl;
    std::cout << "Seconds: " << s.count() << std::endl;
    std::cout << "Milliseconds: " << ms.count() << std::endl;
    std::cout << "Microseconds: " << us.count() << std::endl;
    std::cout << "Nanoseconds: " << ns.count() << std::endl;
    
    auto totalSeconds = std::chrono::duration_cast<std::chrono::seconds>(h + m + s);
    std::cout << "Total seconds: " << totalSeconds.count() << std::endl;
    
    auto totalMinutes = std::chrono::duration_cast<std::chrono::minutes>(h + m);
    std::cout << "Total minutes: " << totalMinutes.count() << std::endl;
}

void test_time_point() {
    std::cout << "\n=== Time Point ===" << std::endl;
    
    auto now = std::chrono::system_clock::now();
    auto epoch = std::chrono::system_clock::from_time_t(0);
    
    auto duration = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    
    std::cout << "Seconds since epoch: " << seconds.count() << std::endl;
    
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current time: " << std::ctime(&now_time);
}

void test_clock() {
    std::cout << "=== Clock Types ===" << std::endl;
    
    auto systemNow = std::chrono::system_clock::now();
    auto steadyNow = std::chrono::steady_clock::now();
    auto highResNow = std::chrono::high_resolution_clock::now();
    
    std::time_t systemTime = std::chrono::system_clock::to_time_t(systemNow);
    std::cout << "System clock: " << std::ctime(&systemTime);
    
    std::cout << "Steady clock is steady: " 
              << (std::chrono::steady_clock::is_steady ? "yes" : "no") << std::endl;
    std::cout << "High-res clock is steady: " 
              << (std::chrono::high_resolution_clock::is_steady ? "yes" : "no") << std::endl;
}

void measure_execution_time() {
    std::cout << "\n=== Measure Execution Time ===" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (volatile int i = 0; i < 1000000; ++i) {
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Loop took " << duration.count() << " microseconds" << std::endl;
}

void test_sleep() {
    std::cout << "\n=== Sleep ===" << std::endl;
    
    auto start = std::chrono::steady_clock::now();
    
    std::cout << "Sleeping for 100 milliseconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    auto end = std::chrono::steady_clock::now();
    auto actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Actually slept for " << actual.count() << " milliseconds" << std::endl;
    
    start = std::chrono::steady_clock::now();
    
    auto wakeTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
    std::cout << "Sleeping until 50ms from now..." << std::endl;
    std::this_thread::sleep_until(wakeTime);
    
    end = std::chrono::steady_clock::now();
    actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Actually slept for " << actual.count() << " milliseconds" << std::endl;
}

void test_literals() {
    std::cout << "\n=== Chrono Literals (C++14) ===" << std::endl;
    
    std::cout << "Note: Chrono literals (2h, 30min, etc.) are C++14 feature" << std::endl;
    
    auto h = std::chrono::hours(2);
    auto min = std::chrono::minutes(30);
    auto sec = std::chrono::seconds(45);
    auto ms = std::chrono::milliseconds(100);
    auto us = std::chrono::microseconds(200);
    auto ns = std::chrono::nanoseconds(300);
    
    std::cout << "2 hours = " << h.count() << " hours" << std::endl;
    std::cout << "30 min = " << min.count() << " minutes" << std::endl;
    std::cout << "45 s = " << sec.count() << " seconds" << std::endl;
    std::cout << "100 ms = " << ms.count() << " milliseconds" << std::endl;
    std::cout << "200 us = " << us.count() << " microseconds" << std::endl;
    std::cout << "300 ns = " << ns.count() << " nanoseconds" << std::endl;
    
    auto total = std::chrono::hours(1) + std::chrono::minutes(30) + std::chrono::seconds(45);
    std::cout << "Total seconds: " 
              << std::chrono::duration_cast<std::chrono::seconds>(total).count() 
              << std::endl;
}

int main() {
    test_duration();
    test_time_point();
    test_clock();
    measure_execution_time();
    test_sleep();
    test_literals();
    return 0;
}
