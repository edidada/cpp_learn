#include <iostream>
#include <type_traits>
#include <utility>

// ====== 兼容 C++14 的 void_t ======
template<typename...>
using void_t = void;

// ====== 检测是否有 .foo() 方法 ======
template<typename T, typename = void>
struct has_foo : std::false_type {};

template<typename T>
struct has_foo<T, void_t<decltype(std::declval<T>().foo())>> : std::true_type {};

// ====== 使用 enable_if 分流调用 ======

// 对于支持 .foo() 的类型
template<typename T>
typename std::enable_if<has_foo<T>::value>::type
call_foo(T& obj) {
  std::cout << "Calling foo()" << std::endl;
  obj.foo();
}

// 对于不支持 .foo() 的类型
template<typename T>
typename std::enable_if<!has_foo<T>::value>::type
call_foo(T&) {
  std::cout << "No foo() method available" << std::endl;
}

// ====== 测试类 ======
struct A {
  void foo() { std::cout << "A::foo" << std::endl; }
};

struct B {};

// ====== 主函数 ======
int main() {
  A a;
  B b;

  call_foo(a); // 输出: Calling foo(), A::foo
  call_foo(b); // 输出: No foo() method available

  return 0;
}