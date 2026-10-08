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

---

## C++14 解决了什么问题？

C++14 是 C++11 的**增量完善版**（"完善 C++11"），修正了 C++11 使用中暴露的繁琐与缺漏：

| 新特性 | 解决的问题 | C++11/之前的方案与不足 |
| --- | --- | --- |
| 泛型 Lambda | Lambda 参数用 `auto`，一份 Lambda 适配多种类型 | C++11 Lambda 必须显式写出参数类型，每种类型写一个 Lambda；或依赖模板函数对象类，样板多 |
| Lambda 捕获初始化（`[x = expr]`） | 捕获表达式结果/移动捕获 | C++11 只能捕获具名变量，无法捕获局部表达式或移动构造对象，只能绕道 `std::bind` |
| 返回值类型推导（`auto` 返回） | 编译器推导函数返回类型 | C++11 必须手写返回类型（常为 `decltype` 表达式，冗长）；无法简洁写通用转换函数 |
| 变量模板（`template<T> constexpr T pi = ...`） | 一份常量模板实例化出多类型 | C++11 只能为每个类型写函数/类模板静态成员，或用宏定义，重复且无类型安全 |
| 泛型 `constexpr` 放宽 | 更复杂的编译期函数 | C++11 `constexpr` 函数体只能单 `return` 语句，几乎无法写分支/循环，实用价值低 |
| `std::make_unique` | 统一创建 `unique_ptr` | C++11 只有 `make_shared`，`unique_ptr` 需裸 `new`（`new Foo(args)`），异常安全瑕疵：`f(unique_ptr<T>(new T), g())` 求值顺序可导致泄漏 |
| `std::integer_sequence` | 编译期整数序列展开 | 元编程手写 `seq<>` 展开技巧，代码晦涩 |
| `std::less<>` 透明比较器（如 `std::less<>` 泛型版本） | 异构键查找 | 默认 `std::less<T>` 限定同类型比较；透明版本允许 `set<long>::find(short)`，减少临时对象构造 |

> 本分支的 `cpp14_sfinae_main.cpp` 正是 C++11/14 时代"成员检测"的核心工具：`void_t` + `decltype` + `std::enable_if`，它解决的痛点是——**模板想要"如果类型有某成员就 A，否则 B"的分派，C++98/03 时代没有可靠手段**，只能依赖重载决议的 SFINAE 规则，写法极难。

## 与 C / Rust 的对比

| 维度 | C | C++14 | Rust |
| --- | --- | --- | --- |
| **类型推导** | 无（一切手写） | `auto`（函数返回值/泛型 Lambda） | `let` + 强类型推断，全面 |
| **编译期计算** | 宏（仅文本替换） | 泛型 `constexpr`（任意函数） | `const fn`（受限，但更安全） |
| **泛型 Lambda** | 无 | C++14 支持 | 闭包天然泛型 |
| **所有权细节** | 手动 | `make_unique` 等安全工厂 | `Box::new`/`Rc::new`，编译期强制 |
| **元编程** | 无 | 模板 + SFINAE（报错体验差） | 过程宏 + Trait（受控、清晰） |
| **安全保证** | 无 | 惯例性 RAII（编译器不强制） | 编译器强制 |

**一句话总结**：C++14 主要解决 C++11 的**"写法繁琐"**问题——类型推导、泛型 Lambda、变量模板让代码更短；用 `make_unique` 堵住裸 `new` 的异常安全缺口。它不引入颠覆性概念，而是把 C++11 打磨顺手；对比 Rust，C++ 的"推导"仍受限于模板的报错可读性，而 Rust 的类型系统在设计上更自洽。
