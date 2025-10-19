#include <iostream>
#include <chrono>
#include <format>  // C++20 format，触发 hh_mm_ss 使用

int main() {
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    // 使用 C++20 格式化时间（触发 hh_mm_ss）
    // 这会间接调用 std::chrono::hh_mm_ss，暴露 GCC 13.1 的 bug
    std::cout << std::format("{:%Y-%m-%d %H:%M:%S}.{:03d}\n",
                             std::chrono::system_clock::from_time_t(time_t),
                             ms.count());

    // 直接构造 hh_mm_ss（更直接触发 bug）
    using namespace std::chrono;
    auto today = floor<days>(now);
    auto tod = time_of_day{now - today};
    // hh_mm_ss 是问题核心
    auto hms = hh_mm_ss{tod};

    std::cout << "Hours: " << hms.hours().count() << "\n";
    std::cout << "Minutes: " << hms.minutes().count() << "\n";
    std::cout << "Seconds: " << hms.seconds().count() << "\n";
    std::cout << "Subseconds: " << hms.subseconds().count() << "\n";

    return 0;
}