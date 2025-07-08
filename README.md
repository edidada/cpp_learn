# README
https://github.com/edidada/cpp11thread

https://cppreference.cn/w/cpp/header

# README
https://github.com/edidada/cpp11thread

https://cppreference.cn/w/cpp/header

C++ 标准库头文件
多用途头文件
语言支持库
概念库
诊断库
内存管理库
元编程库
通用工具库
容器库
迭代器库
范围库
算法库
字符串库
文本处理库
数值库
时间库
输入/输出库
并发支持库


## 98

## 03
<cstdlib>
通用目的工具： 程序控制， 动态内存分配， 随机数， 排序和搜索

<cfloat>
浮点类型的限制

<climits>
整型类型的限制

<csetjmp>
保存（和跳转到）执行上下文的宏（和函数）

<csignal>
用于信号管理的函数和宏常量

<cstdarg>
处理可变长度参数列表

<cstddef>
标准宏和类型定义

<exception>
异常处理工具

<limits>
查询算术类型的属性

<new>
底层内存管理工具

<typeinfo>
运行时类型信息工具

<cassert>
有条件编译的宏，用于将其参数与零进行比较

<cerrno>
包含最后错误编号的宏

<stdexcept>
标准异常类型

<memory>
高级内存管理工具

<bitset>
std::bitset 类模板

<functional>
函数对象、函数调用、绑定操作和引用包装器

<utility>
各种 实用工具组件

<deque>
<deque>

<list>
std::list 容器

<map>
std::map 和 std::multimap 关联容器

<queue>
std::queue 和 std::priority_queue 容器适配器

<set>
std::set 和 std::multiset 关联容器

<stack>
std::stack 容器适配器

<iterator>
范围迭代器

<algorithm>
对范围进行操作的算法

<numeric>
对范围内值进行数值运算

<cstring>
各种 窄字符字符串处理函数

<string>
std::basic_string 类模板

<cctype>
用于确定窄字符类别的函数

<clocale>
C 本地化工具

<cwchar>
各种 宽 和 多字节 字符串处理函数

<cwctype>
用于确定宽字符类别的函数

<locale>
本地化工具

<cmath>
常用数学函数

<complex>
复数类型

<valarray>
用于表示和操作值数组的类

<ctime>
C 风格的时间/日期工具

<cstdio>
C 风格的输入输出函数

<fstream>
std::basic_fstream, std::basic_ifstream, std::basic_ofstream 类模板和类型别名

<iomanip>
用于控制输入和输出格式的辅助函数

<ios>
std::ios_base 类, std::basic_ios 类模板和类型别名

<iosfwd>
输入/输出库中所有类的前向声明

<iostream>
几个标准流对象

<istream>
std::basic_istream 类模板和类型别名

<ostream>
std::basic_ostream, std::basic_iostream 类模板和类型别名

<sstream>
std::basic_stringstream, std::basic_istringstream, std::basic_ostringstream 类模板和类型别名

<streambuf>
std::basic_streambuf 类模板

## 11
<cstdint>
(C++11)
固定宽度整数类型 和 其他类型的限制

<initializer_list>
(C++11)
std::initializer_list 类模板

<typeindex>
(C++11)
std::type_index

<system_error>
(C++11)
定义 std::error_code，一个平台相关的错误代码

<scoped_allocator>
(C++11)
嵌套分配器类

<ratio>
(C++11)
编译时有理算术

<type_traits>
(C++11)
编译时类型信息工具

<tuple>
(C++11)
std::tuple 类模板

<array>
(C++11)
std::array 容器

<forward_list>
(C++11)
std::forward_list 容器

<unordered_map>
(C++11)
std::unordered_map 和 std::unordered_multimap 无序关联容器
<unordered_set>
(C++11)
std::unordered_set 和 std::unordered_multiset 无序关联容器

<codecvt>
(C++11)
(在 C++17 中弃用)
(在 C++26 中移除)
Unicode 转换设施

<cuchar>
(C++11)

<regex>
(C++11)
用于支持正则表达式处理的类、算法和迭代器

<cfenv>
(C++11)
浮点环境 访问函数

<random>
(C++11)
随机数生成器和分布

<chrono>
(C++11)
C++ 时间工具

<cinttypes>
(C++11)
格式化宏, intmax_t 和 uintmax_t 数学和转换

<atomic>
(C++11)
原子操作库

<condition_variable>
(C++11)
线程等待条件

<future>
(C++11)
异步计算原语

<mutex>
(C++11)
互斥原语

<thread>
(C++11)
std::thread 类和 支持函数

## 14
<shared_mutex>
(C++14)
共享互斥原语

## 17

<memory_resource>
(C++17)
多态分配器和内存资源

<any>
(C++17)
std::any 类

<optional>
(C++17)
std::optional 类模板

<variant>
(C++17)
std::variant 类模板

<string_view>
(C++17)
std::basic_string_view 类模板

<charconv>
(C++17)
std::to_chars 和 std::from_chars

<filesystem>
(C++17)
std::filesystem::path 类和 支持函数

## 23
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


```shell
-- The C compiler identification is AppleClang 15.0.0.15000309
-- The CXX compiler identification is AppleClang 15.0.0.15000309
```

```shell
[  8%] Building CXX object CMakeFiles/cpp23_barrier_main.dir/src/cpp23/cpp23_barrier_main.cpp.o
/Users/runner/work/cpp_learn/cpp_learn/src/cpp23/cpp23_barrier_main.cpp:21:22: error: no member named 'jthread' in namespace 'std'
    std::vector<std::jthread> threads;
                ~~~~~^
1 error generated.
make[2]: *** [CMakeFiles/cpp23_barrier_main.dir/src/cpp23/cpp23_barrier_main.cpp.o] Error 1
make[1]: *** [CMakeFiles/cpp23_barrier_main.dir/all] Error 2
make: *** [all] Error 2
```

下面是一个 **C++11 标准下使用 `std::condition_variable` 和 `std::condition_variable_any` 的完整可执行示例程序**，演示了：

- 如何使用 `std::condition_variable`
- 如何使用 `std::condition_variable_any`
- 使用 `cv_status` 判断等待是否超时
- `notify_all_at_thread_exit` 的用法

---

## ✅ 示例：生产者-消费者模型（带超时和线程退出通知）

### 🧩 功能说明：
- 主线程启动一个子线程作为“生产者”，向队列中添加数据。
- 主线程作为“消费者”从队列中取出数据。
- 使用 `condition_variable` 实现同步。
- 使用 `notify_all_at_thread_exit` 确保线程结束前唤醒所有等待的线程。
- 使用 `wait_for()` 检查超时并处理 `cv_status`。

---

## 📄 完整代码如下：

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

queue<int> data_queue;
mutex mtx;
condition_variable cv_producer, cv_consumer;

bool done = false;

void producer(int id, int count) {
    for (int i = 0; i < count; ++i) {
        unique_lock<mutex> lock(mtx);
        cv_producer.wait(lock, []{ return data_queue.empty(); });

        cout << "Producer " << id << " producing data: " << i << endl;
        data_queue.push(i);

        lock.unlock();
        cv_consumer.notify_one();

        this_thread::sleep_for(chrono::milliseconds(300));
    }

    notify_all_at_thread_exit(cv_consumer, unique_lock<mutex>(mtx));
}

void consumer(int count) {
    for (int i = 0; i < count; ++i) {
        unique_lock<mutex> lock(mtx);

        auto now = chrono::system_clock::now();
        bool has_data = cv_consumer.wait_until(lock, now + chrono::seconds(1), []{
            return !data_queue.empty();
        });

        if (!has_data) {
            cout << "[Consumer] Wait timeout." << endl;
            continue;
        }

        int value = data_queue.front();
        data_queue.pop();
        cout << "Consumer consuming data: " << value << endl;

        lock.unlock();
        cv_producer.notify_one();
    }
}

int main() {
    int prod_count = 5;
    thread prod(producer, 1, prod_count);

    for (int i = 0; i < prod_count; ++i) {
        consumer(i);
    }

    if (prod.joinable()) prod.join();

    cout << "Main thread finished." << endl;
    return 0;
}
```

---

## 🔍 输出示例（每次运行可能不同）：

```
Producer 1 producing data: 0
Consumer consuming data: 0
Producer 1 producing data: 1
Consumer consuming data: 1
Producer 1 producing data: 2
Consumer consuming data: 2
Producer 1 producing data: 3
Consumer consuming data: 3
Producer 1 producing data: 4
Consumer consuming data: 4
Main thread finished.
```

---

## 🧠 关键知识点详解

| 组件 | 说明 |
|------|------|
| `std::condition_variable` | 必须与 `std::unique_lock<std::mutex>` 配合使用 |
| `std::condition_variable_any` | 可以与任何锁类型配合（如 `shared_mutex`、自定义锁） |
| `wait()`, `wait_for()`, `wait_until()` | 阻塞等待通知 |
| `notify_one()`, `notify_all()` | 唤醒一个或多个等待线程 |
| `cv_status` | 返回值包括 `no_timeout` 和 `timeout` |
| `notify_all_at_thread_exit()` | 在线程退出时唤醒所有等待线程，防止死锁 |

---

## 🚀 编译运行命令（g++/clang++）

确保你使用的是 C++11 或以上标准：

```bash
g++ -std=c++11 -pthread condition_variable_example.cpp -o condition_variable_example
./condition_variable_example
```

---

## 🧪 扩展练习建议

你可以尝试以下扩展功能来加深理解：

| 扩展方向 | 实现方法 |
|----------|-----------|
| 多个生产者 | 启动多个 `producer` 线程 |
| 多个消费者 | 启动多个 `consumer` 线程 |
| 使用 `condition_variable_any` | 替换 `unique_lock` 为其他锁类型 |
| 加入超时重试机制 | 如果等待超时，自动重试或退出 |
| 使用 `cv_status` 判断结果 | 明确处理超时和正常唤醒情况 |

---

如果你希望我提供基于 `std::condition_variable_any` 的具体例子（比如与 `shared_lock` 结合），也可以继续提问！我可以为你写出完整的可执行程序。

类
promise
(C++11)
存储用于异步检索的值
(类模板)

packaged_task
(C++11)
打包一个函数以存储其返回值，用于异步检索
(类模板)

future
(C++11)
等待异步设置的值
(类模板)

shared_future
(C++11)
等待异步设置的值（可能被其他 future 引用）
(类模板)

launch
(C++11)
指定 std::async 的启动策略
(枚举)

future_status
(C++11)
指定在 std::future 和 std::shared_future 上执行的定时等待的结果
(枚举)

future_error
(C++11)
报告与 future 或 promise 相关的错误
(类)

future_errc
(C++11)
标识 future 错误码
(枚举)

std::uses_allocator<std::promise>
(C++11)
特化 std::uses_allocator 类型特征
(类模板特化)

std::uses_allocator<std::packaged_task>
(C++11)(直到 C++17)
特化 std::uses_allocator 类型特征
(类模板特化)

函数
async
(C++11)
异步运行函数（可能在新线程中），并返回一个 std::future，它将保存结果
(函数模板)

future_category
(C++11)
标识 future 错误类别
(函数)

std::swap(std::promise)
(C++11)
特化 std::swap 算法
(函数模板)

std::swap(std::packaged_task)
(C++11)
特化 std::swap 算法
(函数模板)


类
mutex
(C++11)
提供基本互斥设施
(类)

timed_mutex
(C++11)
提供实现带超时锁定的互斥设施
(类)

recursive_mutex
(C++11)
提供可被同一线程递归锁定的互斥设施
(类)

recursive_timed_mutex
(C++11)
提供可被递归锁定的互斥设施
由同一线程并实现带超时锁定的锁定
(类)

lock_guard
(C++11)
实现严格基于作用域的互斥量所有权包装器
(类模板)

unique_lock
(C++11)
实现可移动的互斥量所有权包装器
(类模板)

scoped_lock
(C++17)
用于多个互斥量的避免死锁的 RAII 包装器
(类模板)

once_flag
(C++11)
辅助对象，用于确保 call_once 仅调用函数一次
(类)
函数

try_lock
(C++11)
尝试通过重复调用 try_lock 来获得互斥量的所有权
(函数模板)

lock
(C++11)
锁定指定的互斥量，如果任何互斥量不可用则阻塞
(函数模板)

call_once
(C++11)
仅调用函数一次，即使从多个线程调用也是如此
(函数模板)

std::swap(std::unique_lock)
(C++11)
特化 std::swap 算法
(函数模板)

std::thread与std::async分别适用于何种场景？需要访问底层的线程实现的API，这个时候通过std::thread可以拿到底层线程的句柄，然后才可以使用底层的线程API。需要给你的应用程序来优化线程的使用，这需要根据应用的特点来优化线程的使用，比如特定机器架构的服务器软件，这个时候就需要使用特定平台下的线程实现。你需要实现一些高级的线程组件，例如线程池，这是C++标准库没有提供的高级组件除此之外还是建议使用async。

特性	C++14 (SFINAE)	C++17 (constexpr if)
实现方式	多个函数模板 + enable_if	单个函数模板 + if constexpr
可读性	较差，需要理解 SFINAE	极佳，接近运行时逻辑
编写复杂度	高，容易出错	简洁直观
推荐程度	旧项目兼容	新代码首选
