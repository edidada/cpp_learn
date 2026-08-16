# C++20 学习示例（cpp20 分支）

> 本分支对应 git 分支 `cpp20`，汇集 C++20 标准新特性代码示例，覆盖 **概念（concepts）**、**协程（coroutines）**、**范围视图（ranges/views）**、**模块（modules）**、**chrono 与格式化** 等主题。

## 核心示例清单（src/cpp20/）

### 一、概念（Concepts）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp20_concepts.cpp` | 概念约束基础 | `requires` 表达式、自定义 concept、`std::integral` 等内建概念、约束模板函数 |

### 二、协程（Coroutines）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp20_coroutine.cpp` | 协程基础框架 | `promise_type`、`co_await`、`coroutine_handle`、`initial_suspend`/`final_suspend`、`suspend_always`/`suspend_never` |
| `cpp20_coroutine2.cpp` | 协程生成器 Generator | `co_yield` 生成序列、`yield_value`、协程句柄生命周期管理（`destroy`） |

### 三、范围与视图（Ranges）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp20_range.cpp` | 范围管道操作 | `views::filter`、`views::transform`、管道操作符 `\|`、惰性求值 |
| `cpp20_chunk.cpp` | `views::chunk` 分块 | 按固定大小切分序列、`std::print` 输出视图 |
| `cpp20_chunk2.cpp` | `istream_view` + `chunk` | `ranges::istream_view` 逐 token 读取流、按 4 个一组组成"数据包" |
| `cpp20_chunk_by.cpp` | `views::chunk_by` 分组 | 按相邻元素谓词分组（如奇偶性相同分一组） |
| `cpp20_remove_prefix.cpp` | `chunk_by` 分割句子 + `remove_prefix` | 按句号切分文本、`string_view::remove_prefix` 去除前导空格 |
| `cpp20_split.cpp` | `views::split` 字符串分割 | 按分隔符拆分字符串视图 |
| `cpp20_split2.cpp` | `views::split` 容器分割 | 按指定元素（如哨兵 `{-1,-1}`）分割 `vector<pair>` 路径点 |

### 四、模块（Modules）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `mymodule.ixx` | 模块接口文件 | `export module`、`export` 导出函数/类、全局模块片段 `module;` |
| `mymodule.cpp` | 模块实现文件 | `module mymodule;`、类成员实现 |
| `main_module.cpp` | 模块使用示例 | `import mymodule;` 导入并调用模块导出符号 |

### 五、chrono 时间与格式化

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `chrono_bug_repro.cpp` | chrono + `std::format` 时间格式化 | `std::format` 格式化 `%Y-%m-%d`、`hh_mm_ss` 拆分时分秒、毫秒输出 |
| `chrono_bug_repro_clang.cpp` | chrono 兼容版本（clang） | `std::put_time`/`localtime` 传统写法、`hh_mm_ss` 解析 `duration`、跨编译器兼容 |

## 编译运行

项目使用 CMake 管理，各示例对应独立的可执行目标，例如：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target cpp20_concepts
```

> 注意：协程（`-fcoroutines`）、模块（`-fmodules-ts`）与 `std::print`/`std::format` 需要较新的编译器（GCC 13+ / Clang 17+ / MSVC 19.38+）支持，且各编译器开关与实现成熟度不同。

## 学习路径建议

1. 概念：`cpp20_concepts`（模板约束入门）
2. 视图：`cpp20_range` → `cpp20_split` → `cpp20_chunk` → `cpp20_chunk_by` → `cpp20_remove_prefix`
3. 协程：`cpp20_coroutine` → `cpp20_coroutine2`（理解 promise/句柄/co_yield 全流程）
4. 模块：`mymodule.ixx` → `mymodule.cpp` → `main_module.cpp`
5. chrono：`chrono_bug_repro_clang` → `chrono_bug_repro`（对比传统与 `std::format` 写法）

---

## C++20 解决了什么问题？

C++20 是继 C++11 之后最大的一次升级，一次性补上 C++ 被诟病多年的四个老大难：

| 新特性 | 解决的问题 | 之前的方案与不足 |
| --- | --- | --- |
| **概念（Concepts）** | 给模板参数加可读、可检查的约束 | SFINAE 报错信息成百上千行、晦涩；`enable_if` 组合爆炸；概念让约束**可命名、可复用、报错清晰** |
| **协程（Coroutines）** | 用同步写法写异步逻辑 | 之前靠回调嵌套（回调地狱）、`std::async`/线程（开销大）、状态机手写；`co_await/co_yield` 让异步代码线性化 |
| **范围视图（Ranges）** | 惰性、可组合、可读的数据管道 | `vector<int> tmp1; for(...) {...}` 手写中间容器；`transform/filter` 需要 `begin/end` 迭代器样板 |
| **模块（Modules）** | 从根上解决头文件问题 | `#include` 文本复制导致：编译慢（O(n²) 重复解析）、宏污染全局、循环依赖难解、封装边界被破坏 |
| **`std::format`** | 类型安全的格式化 | `printf` 格式串类型不匹配即 UB；`<<` 拼接冗长且不可定位；`sprintf` 缓冲区溢出风险 |
| **`std::span`** | 零开销数组视图 | 传 `vector&`/`const T* + size`；`span` 统一"连续内存 + 长度"，且 `std::span<const T>` 可接受任意容器 |
| **`std::jthread`** | 析构自动 join 的线程 | `std::thread` 忘记 `join` 直接析构会 `std::terminate`；手动管理 `join` 异常路径易漏 |
| **三路比较 `<=>`** | 一键生成全部比较运算符 | 之前手写 6 个比较运算符（`< > <= >= == !=`），繁琐易漏 |

## 与 C / Rust 的对比

| 维度 | C | C++20 | Rust |
| --- | --- | --- | --- |
| **泛型约束** | 无（`void*`/宏） | Concepts（可命名约束，报错清晰） | Trait bound（语言核心，报错最佳） |
| **异步** | 回调函数指针 | 协程（`co_await`） | `async/await`（原生一等公民） |
| **数据管道** | 手写循环 | Ranges 视图链（惰性组合） | Iterator + 闭包（`map/filter` 内建） |
| **模块/依赖** | `#include` 头文件 | Modules（编译期隔离） | `mod` + `crate`（模块系统内建） |
| **格式化** | `printf`（类型不安全） | `std::format`（类型安全） | `println!`/`format!`（类型安全宏） |
| **比较运算** | 手写 | `<=>` 自动推导 | 派生宏 `#[derive(PartialOrd)]` |
| **并发** | pthread | `jthread`（自动 join）+ 原子 | `std::thread` + `Send/Sync` 静态检查 |

**一句话总结**：C++20 的四个支柱——**Concepts 驯服模板、Ranges 扁平化算法、协程扁平化异步、Modules 终结头文件地狱**，把 C++ 从"好用但难学"推向"好用且可维护"；对比 Rust，这些能力 Rust 从第一天就以一等公民内建（trait/async/迭代器/mod 系统），C++20 是在兼容旧世界的前提下迎头赶上。
