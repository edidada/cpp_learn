// enable_if03.h —— C++03 手写 enable_if / SFINAE 工具
//
// <type_traits> 里的 enable_if 是 C++11 才进标准。
// C++03 时代库作者手写它，配合 SFINAE 实现"只有满足条件的类型
// 才参与重载/实例化"，是约束模板参数的最经典手段。
#ifndef CPP03_LIB_ENABLE_IF_H
#define CPP03_LIB_ENABLE_IF_H

namespace lib03 {

// enable_if：条件为 true 时提供 type，否则没有 type（触发 SFINAE）
template <bool B, typename T = void>
struct enable_if {
    // 无 type：B == false 时这个主模板会被选中，替换失败 -> 该重载被剔除
};

template <typename T>
struct enable_if<true, T> {
    typedef T type;
};

// 简化版 is_integral（演示用）：完整版见 <type_traits>
template <typename T> struct is_integral_03       { static const bool value = false; };
template <> struct is_integral_03<char>           { static const bool value = true;  };
template <> struct is_integral_03<signed char>    { static const bool value = true;  };
template <> struct is_integral_03<unsigned char>  { static const bool value = true;  };
template <> struct is_integral_03<short>          { static const bool value = true;  };
template <> struct is_integral_03<unsigned short> { static const bool value = true;  };
template <> struct is_integral_03<int>            { static const bool value = true;  };
template <> struct is_integral_03<unsigned int>   { static const bool value = true;  };
template <> struct is_integral_03<long>           { static const bool value = true;  };
template <> struct is_integral_03<unsigned long>  { static const bool value = true;  };

} // namespace lib03

// 便捷宏：把模板函数限制在条件满足时才可用
// 用法：
//   template <typename T>
//   LIB03_REQUIRE(is_integral_03<T>::value, T) twice(T v) { return v * 2; }
#define LIB03_REQUIRE(cond, Ret) \
    typename ::lib03::enable_if<(cond), Ret>::type

#endif // CPP03_LIB_ENABLE_IF_H
