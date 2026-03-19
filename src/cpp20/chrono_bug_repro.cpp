#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

int main() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    std::tm tm = *std::localtime(&time_t);
    std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "." 
              << std::setfill('0') << std::setw(3) << ms.count() << std::endl;

    using namespace std::chrono;
    auto today = floor<days>(now);
    auto tod = now - today;
    auto hms = hh_mm_ss{tod};

    std::cout << "Hours: " << hms.hours().count() << std::endl;
    std::cout << "Minutes: " << hms.minutes().count() << std::endl;
    std::cout << "Seconds: " << hms.seconds().count() << std::endl;
    std::cout << "Subseconds: " << hms.subseconds().count() << std::endl;

    return 0;
}
