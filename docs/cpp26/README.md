# C++26 分支说明（cpp26 分支）

> 本分支对应 git 分支 `cpp26`。目前该分支**尚无 C++26 专属代码示例目录**（`src/cpp26/` 尚未建立），代码主体为 C++11/C++17/C++20/C++23 示例以及 `src/` 根目录下的基础示例，并配置了多版本 CI 工作流。合并 cpp23 后，本分支将聚合 cpp11→cpp23 的全部文档与代码。

## 目录结构

```
src/
├── cpp11/            # C++11 示例（并发、模板分离编译等）
├── cpp17/            # C++17 示例（any/optional/string_view/文件系统等）
├── cpp20/            # C++20 示例（concepts/coroutine/ranges/模块）
├── cpp23/            # C++23 示例（expected/mdspan/stacktrace/spanstream 等）
├── AA.cpp / AA.h     # 类 AA + 友元函数示例
├── NoThisFriendClass_* # 友元类/函数示例
├── constexpr_if.cpp  # if constexpr 编译期分派
├── singleton.cpp     # 双检锁单例模式
├── virtual_main.cpp  # 虚函数多态
├── mymalloc.cpp / simple_alloc_main.cpp # 内存分配器
├── myset_b_tree.cpp  # B 树集合实现
├── isequal_main.cpp  # 模板特化相等判断
├── cpp_err_handler.cpp # 错误处理
└── main.cpp          # 入口示例
```

## 根目录基础示例（src/）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `AA.cpp` / `AA.h` | 类 AA 定义 | 友元函数 `friend`、静态成员、虚析构 |
| `NoThisFriendClass.cpp/.h` + `_main.cpp` | 友元函数访问私有成员 | `friend` 关键字、非成员友元函数 |
| `constexpr_if.cpp` | `if constexpr` 编译期分派 | `std::is_integral_v`、整型/自定义类型分支处理 |
| `singleton.cpp` | 单例模式 | 双重检查锁（DCLP）、`std::mutex`、拷贝禁用 |
| `virtual_main.cpp` | 虚函数多态 | 虚函数、多态、虚析构 |
| `mymalloc.cpp` | 自定义内存分配 | `operator new` 重载、内存管理 |
| `simple_alloc_main.cpp` | 简单分配器 | 分配器实现思路 |
| `myset_b_tree.cpp` | B 树集合 | 数据结构、`std::set` 内部原理 |
| `isequal_main.cpp` | 类型相等判断 | 模板偏特化、`std::is_same` 思想 |
| `cpp_err_handler.cpp` | 错误处理 | 错误码、异常处理 |
| `main.cpp` | 综合入口 | 示例主函数 |

## 各标准子目录（src/cpp11|17|20|23/）

| 标准 | 目录 | 重点内容 |
| --- | --- | --- |
| C++11 | `src/cpp11/` | 条件变量、future/promise/packaged_task、lock_guard/recursive_mutex/try_lock、shared_future、launch 策略、线程局部存储、MyClass 模板分离编译 |
| C++17 | `src/cpp17/` | `std::any`/`optional`/`variant`/`tuple`、`string_view`、`charconv`、filesystem、fstream、INI 解析 |
| C++20 | `src/cpp20/` | concepts、coroutine 协程、ranges 视图、模块（mymodule.ixx） |
| C++23 | `src/cpp23/` | `std::expected`、`mdspan`、`stacktrace`、`spanstream`、`stdfloat`、`barrier` |

> 各子目录的详细示例清单与知识点，见对应 `docs/cpp11|14|17|20|23/README.md`（合并 cpp23 后一并引入）。

## CI 工作流（.github/workflows/）

| 文件 | 说明 |
| --- | --- |
| `cpp17.yml` / `cpp17_macos.yml` | C++17 构建（Ubuntu / macOS） |
| `cpp20.yml` / `cpp20_macos.yml` | C++20 构建（Ubuntu / macOS） |
| `cpp23.yml` / `cpp23_macos.yml` | C++23 构建（Ubuntu / macOS） |
| `cpp23_multiversion.yml` | C++23 多版本编译器矩阵 |
| `ubuntu_latest_debug_cmake-single-platform.yml` | CMake 单平台调试构建 |

## 编译运行

项目使用 CMake 管理，多版本子项目通过 `cmake/` 下的子目录组织：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target cpp17_string_view
```

## 学习路径建议

1. 基础：`src/` 根目录（类、友元、单例、虚函数）→ C++11 并发
2. 进阶：C++17（any/optional/filesystem）→ C++20（concepts/coroutine/ranges）
3. 前沿：C++23（expected/mdspan/stacktrace）→ 关注 C++26 新特性，未来在此分支新增 `src/cpp26/` 示例
