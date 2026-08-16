// type_traits98.h —— C++98 手写类型萃取（库开发元编程起点）
//
// 标准库 <type_traits> 在 C++11 才进入标准。
// 在 C++98 时代，库作者要自己做编译期类型判断，靠的就是"模板偏特化"。
//
// 用法（库内部做重载分派、特化选择时）：
//   template <typename T>
//   void do_copy(T* dst, const T* src) {
//       copy_impl(dst, src, is_pointer<T>());   // 用标签分派
//   }
#ifndef CPP98_LIB_TYPE_TRAITS_H
#define CPP98_LIB_TYPE_TRAITS_H

namespace lib98 {

// ---- integral_constant 的雏形：把编译期常量包装成类型 ----
template <typename T, T v>
struct integral_constant {
    static const T value = v;
    typedef T                 value_type;
    typedef integral_constant<T, v> type;
    operator T() const { return v; }
};

template <bool B>
struct bool_constant : integral_constant<bool, B> {};

typedef bool_constant<true>  true_type;
typedef bool_constant<false> false_type;

// ---- is_same：两个类型是否相同（全特化两个相同类型） ----
template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

// ---- is_void：是否 void（含 const/volatile 限定） ----
template <typename T> struct is_void             : false_type {};
template <> struct is_void<void>                 : true_type {};
template <> struct is_void<const void>           : true_type {};
template <> struct is_void<volatile void>        : true_type {};
template <> struct is_void<const volatile void>  : true_type {};

// ---- is_pointer：是否指针类型（偏特化 T* 匹配所有指针） ----
template <typename T> struct is_pointer      : false_type {};
template <typename T> struct is_pointer<T*>  : true_type {};

// ---- remove_const：去掉 const（偏特化 const T -> T） ----
template <typename T> struct remove_const           { typedef T type; };
template <typename T> struct remove_const<const T>  { typedef T type; };

} // namespace lib98

#endif // CPP98_LIB_TYPE_TRAITS_H
