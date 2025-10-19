#include <iostream>
#include <chrono>
#include <cmath>
#include <format>  // C++20 格式化

int main() {
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto time_temp = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    // C++20 格式化输出（MSVC 19.38+ 支持）
    std::cout << std::format("{:%Y-%m-%d %H:%M:%S}.{:03d}\n",
                             std::chrono::system_clock::from_time_t(time_temp),
                             ms.count());

    // ✅ 使用标准 hh_mm_ss 解析时间（替代 time_of_day）
    using namespace std::chrono;
    auto today = floor<days>(now);
    auto tod = now - today;  // 得到今天过了多久（duration）

    // ✅ 使用 hh_mm_ss 直接解析 duration
    auto hms = hh_mm_ss{tod};  // 这是标准 C++20

    std::cout << "Hours: " << hms.hours().count() << "\n";
    std::cout << "Minutes: " << hms.minutes().count() << "\n";
    std::cout << "Seconds: " << hms.seconds().count() << "\n";
    std::cout << "Subseconds: " << hms.subseconds().count() << "\n";

    return 0;
}