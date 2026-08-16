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
