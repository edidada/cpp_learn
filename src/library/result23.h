// result23.h —— C++23 std::expected 错误处理库模式
//
// 库 API 常见两难：抛异常（性能、控制流隐藏）还是返回错误码（丢失信息）？
// C++23 的 std::expected<T, E> 给出"第三种答案"：像错误码一样显式，
// 但携带类型化错误，且支持函数式链式处理（and_then / transform / or_else）。
//
// 本组件演示一个典型"库 API"：解析字符串为整数，错误用分层枚举表达。
#ifndef CPP23_LIB_RESULT_H
#define CPP23_LIB_RESULT_H

#include <expected>
#include <string>
#include <cstddef>

namespace lib23 {

// ---- 分层错误码：库作者显式声明所有失败路径 ----
enum class ParseError {
    empty,           // 空字符串
    invalid_number,  // 不是合法整数 / 有尾随字符
};

// 主 API：解析成功返回 int，失败返回 ParseError
std::expected<int, ParseError> parse_int(const std::string& s) {
    if (s.empty())
        return std::unexpected(ParseError::empty);

    std::size_t pos = 0;
    int v = 0;
    try {
        v = std::stoi(s, &pos);
    } catch (...) {
        return std::unexpected(ParseError::invalid_number);
    }
    if (pos != s.size())
        return std::unexpected(ParseError::invalid_number);
    return v;
}

// 链式组合：解析成功后继续处理（and_then 保持错误类型不变）
std::expected<int, ParseError> parse_and_double(const std::string& s) {
    return parse_int(s)
        .and_then([](int v) -> std::expected<int, ParseError> {
            return v * 2;
        });
}

// 失败时给默认值
int value_or(const std::string& s, int dflt) {
    return parse_int(s).value_or(dflt);
}

// 失败时换一个错误类型（transform_error 的用法示意）
std::expected<int, std::string> parse_or_msg(const std::string& s) {
    return parse_int(s).transform_error([](ParseError e) {
        return e == ParseError::empty ? std::string("input is empty")
                                      : std::string("invalid number");
    });
}

} // namespace lib23

#endif // CPP23_LIB_RESULT_H
