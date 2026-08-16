// static_assert98.h —— C++98 编译期断言（static_assert 是 C++11 才有）
//
// 库代码经常需要"如果模板参数不满足约定就编译失败"。
// C++98 没有 static_assert，常用技巧：
//   1) 数组尺寸为 -1（负尺寸非法）
//   2) 常量表达式除零 1/0
// 二者都会产生一个带明显报错信息的编译错误。
#ifndef CPP98_LIB_STATIC_ASSERT_H
#define CPP98_LIB_STATIC_ASSERT_H

namespace lib98 {

// 先做一层包装：true 时 value = 1，false 时没有 value（触发错误）
template <bool>
struct compile_time_assertion;

template <>
struct compile_time_assertion<true> {
    static const int value = 1;
};

} // namespace lib98

// 用法：LIB98_STATIC_ASSERT(条件, 提示文字)
// 注意：__LINE__ 保证同一行展开多次也不重名；不同行可用相同条件。
#define LIB98_STATIC_ASSERT(cond, msg) \
    typedef char lib98_static_assert_##__LINE__ \
        [(::lib98::compile_time_assertion<(cond)>::value) ? 1 : -1]

// 除零版本（可选）：报错信息更直白，但只能用在能求值的地方
#define LIB98_STATIC_ASSERT_ALT(cond) \
    enum { lib98_sa_##__LINE__ = 1 / int((cond) ? 1 : 0) }

#endif // CPP98_LIB_STATIC_ASSERT_H
