# C++23 学习示例（cpp23 分支）

> 本分支对应 git 分支 `cpp23`，汇集 C++23 标准新特性代码示例，覆盖 **std::expected 错误处理**、**std::mdspan 多维视图**、**std::stacktrace 堆栈跟踪**、**std::barrier 线程屏障**、**std::spanstream 流式读写**、**std::stdfloat 浮点类型**、**多维下标运算符** 等主题。

## 核心示例清单（src/cpp23/）

### 一、错误处理

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp23_expected_example.cpp` | `std::expected` 错误处理 | 返回值携带错误、`std::unexpected`、`operator bool`、`.value()`/`.error()` 访问 |

### 二、内存与视图

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp23_mdspan_example.cpp` | `std::mdspan` 多维数组视图 | `std::dextents` 动态维度、`extent()` 获取维度、`m(i,j)` 下标访问 |
| `cpp23_multi_dimensional_subscript_operator.cpp` | 多维下标运算符 | `operator[](size_t i, size_t j)` 多参数重载、Matrix 类模拟 |

### 三、并发与协程

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp23_barrier_main.cpp` | `std::barrier` 线程屏障 | `arrive_and_wait` 同步、`std::jthread` 自动汇合、多线程分段执行 |
| `cpp23_corouting_opt.cpp` | 协程 Task 封装 | `promise_type`、`co_return`、`coroutine_handle::resume` |

### 四、诊断与调试

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp23_stacktrace_example.cpp` | `std::stacktrace` 堆栈跟踪 | `std::stacktrace::current()` 获取调用栈、流式输出 |
| `cpp23_assume.cpp` | `[[assume]]` 优化提示属性 | 编译器假定前提、`assert` 配合使用、优化潜力 |

### 五、IO 与格式化

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp23_spanstream_example.cpp` | `std::spanstream` 流式读写 | `std::ospanstream` 输出到 `std::vector<char>` 缓冲、`.span()` 获取已写区域 |
| `cpp23_stdfloat_example.cpp` | `std::stdfloat` 固定宽度浮点 | `std::float32_t`、`1.0f32` 字面量、固定精度类型 |
| `cpp23_remove_prefix.cpp` | ranges 句子分割 + `remove_prefix` | `views::chunk_by` 分割、`string_view::remove_prefix` 去空格 |
| `cpp23_split2.cpp` | `views::split` 容器分割 | 按哨兵元素分割 `vector<pair>` 路径段 |

> 说明：`cpp23_remove_prefix.cpp` 与 `cpp23_split2.cpp` 主要使用 C++20 ranges 特性，此处作为 C++23 环境下 ranges 的应用练习。

## 编译运行

项目使用 CMake 管理，各示例对应独立的可执行目标，例如：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target cpp23_expected_example
```

> 注意：`<stdfloat>`、`<mdspan>`、`<stacktrace>` 等头文件需要较新编译器（GCC 13+/Clang 17+/MSVC 19.33+）且在不同平台上支持程度不一，部分示例（如 stacktrace）在特定编译环境下可能无法链接（编译时可能报 `Linking CXX executable` 错误）。

## 学习路径建议

1. 错误处理：`cpp23_expected_example`（现代 C++ 推荐的错误返回方式）
2. 多维数组：`cpp23_multi_dimensional_subscript_operator` → `cpp23_mdspan_example`
3. 并发：`cpp23_barrier_main` → `cpp23_corouting_opt`
4. 调试工具：`cpp23_stacktrace_example` → `cpp23_assume`
5. IO：`cpp23_spanstream_example` → `cpp23_stdfloat_example`

## FAQ：stacktrace 输出中的 `D:\a\` 路径从哪来？

运行 `cpp23_stacktrace_example.exe` 时，`std::stacktrace` 打印的调用栈里会出现类似下面的帧：

```
...
D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp(15,1)
D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl(339,1)
...
```

**这不是错误，也与你的环境无关。**

- `D:\a\_work\1\s\...` 是微软内部构建机的路径（Azure DevOps 构建代理的默认工作目录），MSVC CRT（C 运行时库）的启动代码（`exe_main.cpp`、`exe_common.inl` 等）在微软构建机上编译时，PDB 调试符号里记录的就是这条源码路径。
- 因此只要 stacktrace 落到 CRT 的 `mainCRTStartup` → `__scrt_common_main` → `main` 的启动链路上，就会出现 `D:\a\` 前缀的帧。所有使用官方 MSVC 预编译运行时库的机器都会这样，属于正常现象。

### 只显示自己的代码帧

如果不想看到 CRT 内部帧，可以按源码路径过滤，只保留项目内的代码：

```cpp
#include <stacktrace>

void dump_user_frames() {
    for (const auto& frame : std::stacktrace::current()) {
        const auto& src = frame.source_file();
        // 只打印位于本项目目录下的帧
        if (!src.empty() && src.starts_with("D:\\develops")) {
            std::cout << frame << '\n';
        }
    }
}
```

> 提示：不同机器上项目所在盘符/路径不同，过滤条件可改成相对判断（如只保留 `cpp23_stacktrace_example.cpp` 所在的帧），以兼容跨机器环境。
