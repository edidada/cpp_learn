# src/library —— 用 C++ 各版本特性做库开发

本目录按 C++ 版本分支组织，每个子目录/文件是**可复用的库组件代码**（以 header-only 为主），
演示该版本新增的语言特性在**库开发**（写容器、分配器、类型萃取、RAII 工具、错误处理 API 等）中的实际用法。

> 说明：本分支是 `cpp98`，这里只放 C++98 标准能编译的库代码；
> 更高版本（cpp11/14/17/20/23）的库代码在各自分支的 `src/library/` 中，并通过分支合并链向上累积。

## 本分支组件清单（C++98）

| 文件 | 组件 | 解决的问题 |
| --- | --- | --- |
| `type_traits98.h` | 手写类型萃取（`is_same`/`is_void`/`is_pointer`/`remove_const`） | 库代码常用"编译期类型判断 + 重载/特化分派"，标准库 `<type_traits>` 在 C++11 才进标准，98 时代靠模板偏特化手写 |
| `static_assert98.h` | 编译期断言宏 | `static_assert` 关键字是 C++11 才有的，库代码在 98 下用"负尺寸数组/除零常量表达式"的技巧做编译期检查 |
| `main98.cpp` | 用法演示 | 编译运行示例 |

## 编译与运行

```bash
# MinGW / MSVC 均可用，指定 C++98 标准
g++ -std=c++98 -Wall -Wextra src/library/main98.cpp -o /tmp/lib98_demo
/tmp/lib98_demo
```

## 这些特性对库开发意味着什么

- **模板偏特化（Partial Specialization）**：类型萃取、`is_pointer<T*>` 这类"对特定形态做专门处理"全靠它，是元编程的基石。
- **模板全特化（Explicit Specialization）**：`is_void<void>` 等对具体类型的特例。
- **类模板/函数模板**：容器、分配器、RAII 守卫的类型无关代码。
- **编译期求值**：`enum { ... }`、`static const int`、模板参数求值，让"库的契约"在编译期就能被检查（配合上面的静态断言宏）。
- **不足（为什么后来要改）**：没有 `static_assert`、`nullptr`、`auto`、右值引用、`>>` 尖括号，代码冗长；类型萃取全靠手写。

与 C / Rust 对比：C 的库依赖宏 + 运行时错误；Rust 有内置 `const fn` + trait 约束（类似 concepts，是 C++20 才有）；C++98 只能在"模板特化 + 宏"里变通。
