# C++14 学习示例（cpp14 分支）

> 本分支对应 git 分支 `cpp14`，汇集 C++14 标准相关代码示例，重点覆盖 **函数对象比较器（std::less/std::greater）**、**SFINAE 模板检测**、**智能指针所有权语义** 以及 **多线程互斥示例** 等主题。

## 核心示例清单（src/cpp14/）

### 一、比较器（std::less / std::greater）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `comparator_examples.h` | 比较器示例头文件 | 声明 `std::sort` 与 `std::priority_queue` 的比较器演示函数 |
| `cpp14_greater_less_example.cpp` | 排序与优先队列中的比较器 | `std::sort` 升降序、`std::priority_queue` 小顶堆/大顶堆、`std::less`/`std::greater` 函数对象 |
| `cpp14_std_less.cpp` | `std::less` 全面用法 | 函数对象直接调用、`std::sort` 显式指定、`std::set`/`std::map` 默认排序规则 |

### 二、SFINAE 模板元编程

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp14_sfinae_main.cpp` | SFINAE 成员检测 + 重载分流 | `void_t` 惯用法、`decltype` + `std::declval`、`std::enable_if`、`std::false_type`/`std::true_type`、编译期类型特征检测 |

### 三、智能指针所有权语义（ownership/）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `ownership.cpp` | 独占所有权转移 | `std::unique_ptr` 按值传参、`std::move` 转移、函数结束自动析构 |
| `pass_ownership.cpp` | 工厂函数返回 + 所有权传递 | `make_unique` 创建、函数返回值转移所有权、`move` 传给处理函数 |
| `share_ownership.cpp` | 共享所有权 | `std::shared_ptr`、`use_count()` 引用计数、多函数共享资源 |
| `observer.cpp` | 观察者模式防悬垂 | `std::weak_ptr` 观察者注册、`lock()` 安全访问、避免循环引用 |
| `temporary_borrowing.cpp` | 临时借用（不转移所有权） | 引用传参 `const&`、借用不拥有、生命周期管理 |
| `advance_ownership.cpp` | 自定义删除器 | `std::unique_ptr<T, Deleter>`、`fopen`/`fclose` RAII 封装、异常安全 |

### 四、多线程互斥（example/）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `6-3-1.cpp` | pthread 多线程互斥累加（原始版） | `pthread_mutex_t` 初始化/加锁/解锁、`pthread_create`/`pthread_join`、竞态控制 |
| `6-3-1_success.cpp` | 修正版 | 错误返回码检查（`ret != 0`）、`exit(EXIT_FAILURE)`、`return nullptr` 规范化 |

> 说明：`example/` 下的示例基于 POSIX pthread（非 C++11 标准线程），用于演示操作系统级线程同步的基本原理；更现代的写法见 `src/cpp11/` 中的 `std::thread`/`std::mutex` 系列。

## 编译运行

项目使用 CMake 管理，各示例对应独立的可执行目标，例如：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target cpp14_sfinae_main
```

## 学习路径建议

1. 比较器：`cpp14_std_less` → `cpp14_greater_less_example`（理解函数对象与 STL 排序）
2. SFINAE：`cpp14_sfinae_main`（模板检测是理解现代 C++ 元编程的基础）
3. 所有权：`ownership.cpp` → `pass_ownership` → `share_ownership` → `observer` → `advance_ownership`
4. 线程互斥：`example/6-3-1.cpp` 对比 `6-3-1_success.cpp`（错误处理的最佳实践）
