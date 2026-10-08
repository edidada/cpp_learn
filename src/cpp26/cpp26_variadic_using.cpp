#include <iostream>
#include <string>

// C++26 特性：变参 using 声明 —— using Ts::name...;
// 一次从基类包中引入所有同名成员，替代手写多个 using 或用继承构造函数的取巧写法
// （C++17 只允许 using Base::Base... 即继承构造函数；引入普通成员函数的包展开是 C++26 新增）
// 实测：MSVC 14.51 (VS 2026 18.10) /std:c++latest 通过

// —— 经典场景 1：重载集合（多个 functor 合成一个具有全部 operator() 的对象）——
template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;   // C++26：一行引入所有基类的 operator()
};

struct AsInt   { int    operator()(int v)                const { return v; } };
struct AsParse { int    operator()(const std::string& s) const { return std::stoi(s); } };
struct AsZero  { int    operator()(std::nullptr_t)       const { return 0; } };

// —— 经典场景 2：同名不同签名的成员函数聚合 ——
struct LoggerA { void log(int i)    { std::cout << "LoggerA::log(int) "    << i   << std::endl; } };
struct LoggerB { void log(double d) { std::cout << "LoggerB::log(double) " << d   << std::endl; } };
struct LoggerC { void log(const std::string& s) { std::cout << "LoggerC::log(string) " << s << std::endl; } };

template <typename... Ts>
struct MultiLogger : Ts... {
    using Ts::log...;          // C++26：变参 using 引入成员函数
};

int main() {
    // 场景 1：像 pattern matching 一样的重载调用集
    Overload o{AsInt{}, AsParse{}, AsZero{}};
    std::cout << "o(42)      = " << o(42)      << std::endl;   // -> AsInt
    std::cout << "o(\"123\")   = " << o("123")   << std::endl;   // -> AsParse
    std::cout << "o(nullptr) = " << o(nullptr) << std::endl;   // -> AsZero

    // 场景 2：三个日志器聚合成一个按重载解析分派的出口
    MultiLogger<LoggerA, LoggerB, LoggerC> m;
    m.log(7);
    m.log(3.14);
    m.log("cpp26 variadic using");

    return 0;
}
