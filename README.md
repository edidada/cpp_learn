# README
https://github.com/edidada/cpp11thread

https://cppreference.cn/w/cpp/header

<compare> (C++20) 三路比较运算符 支持
<coroutine> (C++20) 协程支持库
<source_location> (C++20) 提供获取 源代码位置 的方法
<version> (C++20) 提供用于验证库实现状态的宏
<concepts> (C++20) 基本库概念
<bit> (C++20) 位操作 函数
<span> (C++20) std::span 视图
<ranges> (C++20) 范围访问、原语、要求、工具和适配器
<format> (C++20) 格式化库，包括 std::format
<numbers>(C++20) 数学常数
<syncstream> (C++20) std::basic_osyncstream, std::basic_syncbuf 和类型别名
<barrier> (C++20) 屏障
<latch> (C++20) 闩锁
<semaphore> (C++20) 信号量
<stop_token> (C++20) 用于 std::jthread 的停止令牌
<format> (C++20) 格式化库，包括 std::format

## 23
<stdfloat> (C++23) 固定宽度浮点类型
<stacktrace> (C++23) 堆栈跟踪 库
<expected> (C++23) std::expected 类模板
<flat_map> (C++23) std::flat_map 和 std::flat_multimap 容器适配器
<flat_set> (C++23) std::flat_set 和 std::flat_multiset 容器适配器
C++23 中并没有直接引入 std::flat_map 和 std::flat_set，但可以使用第三方库如 Boost 提供的类似功能作为替代。

<mdspan> (C++23) std::mdspan 视图
<generator> (C++23) std::generator 类模板
<print> (C++23) 格式化输出库，包括 std::print
<spanstream> (C++23) std::basic_spanstream, std::basic_ispanstream, std::basic_ospanstream 类模板和类型别名

```shell
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0 报错 [ 50%] Linking CXX executable cpp23_stacktrace_example
```

```shell
/usr/bin/ld: CMakeFiles/cpp23_stacktrace_example.dir/src/cpp23/cpp23_stacktrace_example.cpp.o: in function `std::stacktrace_entry::_S_init()':
/usr/include/c++/13/stacktrace:164:(.text._ZNSt16stacktrace_entry7_S_initEv[_ZNSt16stacktrace_entry7_S_initEv]+0x53): undefined reference to `__glibcxx_backtrace_create_state'
/usr/bin/ld: CMakeFiles/cpp23_stacktrace_example.dir/src/cpp23/cpp23_stacktrace_example.cpp.o: in function `std::stacktrace_entry::_M_get_info(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, int*) const':
/usr/include/c++/13/stacktrace:196:(.text._ZNKSt16stacktrace_entry11_M_get_infoEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_Pi[_ZNKSt16stacktrace_entry11_M_get_infoEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_Pi]+0x9b): undefined reference to `__glibcxx_backtrace_pcinfo'
/usr/bin/ld: /usr/include/c++/13/stacktrace:206:(.text._ZNKSt16stacktrace_entry11_M_get_infoEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_Pi[_ZNKSt16stacktrace_entry11_M_get_infoEPNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES6_Pi]+0x108): undefined reference to `__glibcxx_backtrace_syminfo'
/usr/bin/ld: CMakeFiles/cpp23_stacktrace_example.dir/src/cpp23/cpp23_stacktrace_example.cpp.o: in function `std::basic_stacktrace<std::allocator<std::stacktrace_entry> >::current(std::allocator<std::stacktrace_entry> const&)':
/usr/include/c++/13/stacktrace:259:(.text._ZNSt16basic_stacktraceISaISt16stacktrace_entryEE7currentERKS1_[_ZNSt16basic_stacktraceISaISt16stacktrace_entryEE7currentERKS1_]+0x76): undefined reference to `__glibcxx_backtrace_simple'
collect2: error: ld returned 1 exit status
gmake[2]: *** [CMakeFiles/cpp23_stacktrace_example.dir/build.make:101: cpp23_stacktrace_example] Error 1
gmake[1]: *** [CMakeFiles/Makefile2:166: CMakeFiles/cpp23_stacktrace_example.dir/all] Error 2
gmake: *** [Makefile:91: all] Error 2
```
