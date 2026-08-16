// chrono_bug_repro_clang.cpp
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

int main() {
    auto now = std::chrono::system_clock::now();

    // 使用标准方法获取 time_t
    auto time_temp = std::chrono::system_clock::to_time_t(now);

    // 获取毫秒部分
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    // 打印时间（使用传统方式，避免 std::format 兼容性问题）
    std::cout << std::put_time(std::localtime(&time_temp), "%Y-%m-%d %H:%M:%S")
              << "." << std::setfill('0') << std::setw(3) << ms.count() << "\n";

    // 使用标准 hh_mm_ss 解析时间
    using namespace std::chrono;
    auto today = floor<days>(now);  // 截断到天
    auto duration_since_midnight = now - today;  // 今天过了多久

    // 直接构造 hh_mm_ss
    auto hms = hh_mm_ss{duration_since_midnight};

    std::cout << "Hours: " << hms.hours().count() << "\n";
    std::cout << "Minutes: " << hms.minutes().count() << "\n";
    std::cout << "Seconds: " << hms.seconds().count() << "\n";
    std::cout << "Subseconds: " << hms.subseconds().count() << "\n";

    return 0;
}