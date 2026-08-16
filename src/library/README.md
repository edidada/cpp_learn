# src/library —— 用 C++ 各版本特性做库开发

本目录按 C++ 版本分支组织，每个文件是**可复用的库组件代码**（以 header-only 为主），
演示该版本新增的语言特性在**库开发**（写容器、分配器、类型萃取、RAII 工具、错误处理 API 等）中的实际用法。

> 本目录内容沿分支合并链向上累积（`cpp98 → cpp03 → cpp11 → ... → cpp23`），
> 高版本分支会包含所有低版本分支的库组件；每个组件的源文件前缀标明所属版本。

## 组件清单（按版本累积）

### C++98

| 文件 | 组件 | 解决的问题 |
| --- | --- | --- |
| `type_traits98.h` | 手写类型萃取（`is_same`/`is_void`/`is_pointer`/`remove_const`） | 库代码常用"编译期类型判断 + 重载/特化分派"，标准库 `<type_traits>` 在 C++11 才进标准，98 时代靠模板偏特化手写 |
| `static_assert98.h` | 编译期断言宏 | `static_assert` 关键字是 C++11 才有的，库代码在 98 下用"负尺寸数组/除零常量表达式"的技巧做编译期检查 |
| `main98.cpp` | C++98 组件用法演示 | 编译运行示例 |

### C++03

| 文件 | 组件 | 解决的问题 |
| --- | --- | --- |
| `enable_if03.h` | 手写 `enable_if` + 简化 `is_integral` | SFINAE 重载决议：让库的模板函数只在满足类型条件时才参与重载（`<type_traits>` 的 `enable_if` 是 C++11 才进标准，03 时代库作者手写） |
| `scope_guard03.h` | RAII ScopeGuard | 离开作用域自动执行清理（关文件/解锁/释放资源），是异常安全库代码的基石；C++03 没有移动语义，用"复制即转移"模拟 |
| `main03.cpp` | C++03 组件用法演示 | 编译运行示例 |

### C++11

| 文件 | 组件 | 解决的问题 |
| --- | --- | --- |
| `move_string.h` | 带移动语义的 String（移动构造/移动赋值/swap/`noexcept`） | 库中"值类型"设计的核心：右值引用让拷贝变移动，`std::vector` 扩容、函数按值返回从此零拷贝 |
| `mini_shared_ptr.h` | 简易 `shared_ptr`（引用计数 + 控制块） | 库级所有权管理：多个对象共享一个资源，自动释放；演示 `nullptr`、`noexcept`、`explicit operator bool` 等 C++11 细节 |
| `main11.cpp` | C++11 组件用法演示 | 编译运行示例 |

### C++14

| 文件 | 组件 | 解决的问题 |
| --- | --- | --- |
| `index_sequence14.h` | 手写 `integer_sequence`/`make_index_sequence` + tuple 打印 / `apply` / 数组展开 | 编译期整数序列是 C++14 库元编程的"展开包"利器：把运行时才知道个数的参数（tuple、数组、模板包）在编译期逐一拆开处理 |
| `main14.cpp` | C++14 组件用法演示 | 编译运行示例 |

## 编译与运行

```bash
# MinGW / MSVC 均可用，按对应标准编译
g++ -std=c++98  -Wall -Wextra src/library/main98.cpp  -o /tmp/lib98_demo && /tmp/lib98_demo
g++ -std=c++03  -Wall -Wextra src/library/main03.cpp  -o /tmp/lib03_demo && /tmp/lib03_demo
g++ -std=c++11  -Wall -Wextra src/library/main11.cpp  -o /tmp/lib11_demo && /tmp/lib11_demo
g++ -std=c++14  -Wall -Wextra src/library/main14.cpp  -o /tmp/lib14_demo && /tmp/lib14_demo
```

## 这些特性对库开发意味着什么

### C++98

- **模板偏特化（Partial Specialization）**：类型萃取、`is_pointer<T*>` 这类"对特定形态做专门处理"全靠它，是元编程的基石。
- **模板全特化（Explicit Specialization）**：`is_void<void>` 等对具体类型的特例。
- **类模板/函数模板**：容器、分配器、RAII 守卫的类型无关代码。
- **编译期求值**：`enum { ... }`、`static const int`、模板参数求值，让"库的契约"在编译期就能被检查（配合上面的静态断言宏）。
- **不足（为什么后来要改）**：没有 `static_assert`、`nullptr`、`auto`、右值引用、`>>` 尖括号，代码冗长；类型萃取全靠手写。

与 C / Rust 对比：C 的库依赖宏 + 运行时错误；Rust 有内置 `const fn` + trait 约束（类似 concepts，是 C++20 才有）；C++98 只能在"模板特化 + 宏"里变通。

### C++03

- **RAII（资源获取即初始化）**：C++03 时代的库（如 `std::auto_ptr`、`std::ofstream`）靠构造函数拿资源、析构函数释放资源；库接口只需返回对象，调用方无需手工 `close/free`。
- **SFINAE（替换失败不是错误）+ 偏特化**：`enable_if` 的底层机制，是重载分派、约束模板的经典手段。
- **函数对象（仿函数）**：没有 lambda 的 C++03 里，库的回调/策略全靠自定义仿函数（`scope_guard` 的例子就是）。
- **不足（为什么后来要改）**：
  - 没有移动语义，`auto_ptr` 的"复制即转移"语义是反直觉的（`std::unique_ptr` 到 C++11 才修复）；
  - 没有 lambda，回调代码冗长；
  - `enable_if` 手写、报错信息难懂。

与 C / Rust 对比：C 的库靠"约定 + 宏"，资源清理容易漏（没有 RAII）；Rust 的 RAII（Drop）从 1.0 就是语言一等公民，所有权/移动是语言机制；C++03 只能靠 `scope_guard` 之类的库技巧弥补。

### C++11

- **右值引用 / 移动语义**：库的"值类型"（String/Vector/Optional 等）从此可以在扩容、按值返回、容器操作时零拷贝转移资源；C++03 的 `auto_ptr` 反直觉所有权转移被 `unique_ptr`/`shared_ptr` 取代。
- **智能指针**：`unique_ptr`（独占所有权）、`shared_ptr`（共享所有权 + 引用计数）让库接口能表达"谁拥有资源"，资源生命周期由类型系统管理。
- **`noexcept`**：标记不会抛异常的操作（移动、swap），容器才敢在强异常安全保证下使用它们。
- **可变参数模板 / 完美转发 / `nullptr` / `auto` / lambda / `constexpr`**：库接口更简洁，回调不必再写仿函数类，元编程（如 `index_sequence` 的前身）开始成形。
- **不足（为什么后来要改）**：还没有 Concepts（约束写在 `enable_if` 里，报错信息难懂）、没有协程、没有模块、元编程实现复杂。

与 C / Rust 对比：C 里移动/所有权全靠指针 + 约定；Rust 把移动（move）和所有权（ownership）做成语言机制；C++11 的移动语义给库作者提供了"性能与安全兼得"的工具，但需要自己遵守规范（如"移动后保持有效但未指定状态"）。

### C++14

- **`integer_sequence` / `index_sequence`**：把"0..N-1"变成编译期常量序列，配合参数包展开做 tuple 打印、`apply`、按索引访问——是 C++14 库元编程的标准工具。
- **返回类型推导 / 泛型 lambda / 变量模板**：库代码更短；`std::make_unique`（异常安全的工厂）补齐了 `std::make_shared` 的缺失。
- **不足（为什么后来要改）**：展开参数包还要用"初始化列表逗号展开"这种技巧（`expander{0, (...)...}`），C++17 的折叠表达式直接一行搞定；没有 `if constexpr`、没有结构化绑定。
