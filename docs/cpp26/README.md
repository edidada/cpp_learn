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

---

# Windows / MSVC 实测补充（2026-09-25 追加，cpp26_win 分支）

> 上文"尚无 `src/cpp26/` 示例目录"的表述自本节起已过时：`src/cpp26/` 已建立。本节在 **Windows + Visual Studio 2026 (v18.10) / MSVC 14.51.36231** 环境下，对 C++26 特性做了**逐个编译实测**（`cl /std:c++latest`），并把验证过的特性固化为示例代码。所有"✅ 已验证"均有对应可运行代码，"❌ 未支持"均为实测编译/预处理失败的结果。

## 实测环境

| 项 | 值 |
| --- | --- |
| OS | Windows 11 x64 |
| IDE | Visual Studio Enterprise 2026 (18.10.12217.157) |
| 工具集 | MSVC 14.51.36231（vcvars64） |
| 开关 | `/std:c++latest`（注意：**`/std:c++26` 尚不存在**，传了会被忽略并告警 D9002） |
| STL | MSVC 内置（`__cpp_lib_print = 202406`） |

手工编译：

```bat
"C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
cl /std:c++latest /EHsc src\cpp26\cpp26_variadic_using.cpp
```

或走 CMake（`cmake/cpp26/`，MSVC 下自动追加 `/std:c++latest`，并守卫工具集版本 ≥ 14.50）：

```bat
cmake -B build-cpp26
cmake --build build-cpp26 --target cpp26_variadic_using
```

## C++26 特性支持矩阵（MSVC 14.51 实测）

### ✅ 已验证可用（有示例代码）

| 特性 | 提案 | 示例文件 | 一句话说明 |
| --- | --- | --- | --- |
| lambda 捕获结构化绑定 | P2034 | `cpp26_structured_binding_capture.cpp` | `[x, y]` 直接捕获 `auto [x,y]`，不用再绕聚合体 |
| 衰减复制 `auto(expr)` | P0849 | `cpp26_decay_copy.cpp` | 显式把引用/数组/函数"拍成值"，模板里告别 `std::decay_t` |
| 变参 using 声明 | P2662 配套 (variadic using) | `cpp26_variadic_using.cpp` | `using Ts::operator()...;` 一行聚合重载集合 |
| constexpr 容器（vector/string） | C++26 库 | `cpp26_constexpr_vector.cpp` | 编译期筛素数、编译期排序；注意堆内存须在同一常量求值内释放 |
| `std::print` 到 `FILE*` | C++26 库 | `cpp26_print_file.cpp` | `std::print(stderr/fptr, "{}", v)`，格式化输出打通 C 流 |
| 契约语法（MSVC 括号形式） | P2900 | `cpp26_contracts.cpp` | `[[pre(expr)]]` / `[[contract_assert(expr)]]` / `[[contract_semantics]]` 可解析，**运行时求值尚未启用**（见下） |

### ❌ 实测未支持（18.10 stable / 14.51）

| 特性 | 提案 | 实测症状 |
| --- | --- | --- |
| Pack indexing（包下标 `Ts...[0]`） | P2662 | C3520/C3522：参数包无法在此上下文展开 |
| 变参友元 `friend Ts...;` | P2893 | C2059 语法错误 |
| `#embed` 预处理器嵌入二进制 | P1967 | C1021：无效的预处理器命令 "embed" |
| static_assert 用户生成消息 | P2741 | `__cpp_static_assert` 仍为 201411，constexpr `std::string` 消息报错 |
| constexpr 定位 new | — | C2131：常量求值中 `::new (buf) int(42)` 不可用 |
| 静态反射 | P2996 | `__cpp_reflection` 未定义，`<meta>` 走不到 |
| 契约定稿冒号语法 `[[pre: expr]]` | P2900R14 | C2760：应为 `]` |
| `std::execution`（P2300 sender/receiver） | — | `__cpp_lib_execution` 仍为 201902（旧并行算法值），无执行器库 |

> 提示：`[[contract_assert]]`、`[[contract_semantics]]` 等注解在 18.10 中**可编译但不强制执行**——实测 `half(-5)` 违反 `[[pre(n >= 0)]]` 后照常返回。等待 MSVC 后续版本对齐 P2900 定稿语义；当前跨编译器项目通常仍用 `assert` / 手写检查宏过渡。

## src/cpp26/ 示例清单

| 文件 | 演示要点 |
| --- | --- |
| `cpp26_structured_binding_capture.cpp` | 值捕获/引用捕获结构化绑定、范围 for + lambda、map 遍历 |
| `cpp26_decay_copy.cpp` | `auto(rx)` 值化、数组→指针、函数→函数指针、拷贝隔离、泛型按值转发 |
| `cpp26_variadic_using.cpp` | `using Ts::operator()...` 重载集合（模拟模式匹配）、`using Ts::log...` 聚合日志器 |
| `cpp26_constexpr_vector.cpp` | 编译期埃氏筛求和、编译期 snake→Pascal 字符串、编译期排序返回 `std::array` |
| `cpp26_print_file.cpp` | print 到 stdout/stderr/文件 `FILE*`/`std::ostream` 四种目标 + 格式化对齐 |
| `cpp26_contracts.cpp` | `[[pre]]`/`[[contract_assert]]`/评估模式注解的当前支持边界 |

## 踩坑记录（重要）

1. **旧工具集会 ICE**：MSVC 14.38（VS 2022 自带组件）解析 `auto(expr)` 等新语法时不是报语法错误，而是**内部编译器错误 C1001**。`cmake/cpp26/CMakeLists.txt` 已加 `MSVC_VERSION < 1950` 的 FATAL_ERROR 守卫，源文件也有 `#error` 保护；CLion 的 CMake profile 务必绑定 v18 工具链（`C:\Program Files\Microsoft Visual Studio\18\Enterprise`）。
2. **constexpr vector 的生命周期限制**：MSVC 目前要求常量求值中的堆内存**在同一条常量表达式内释放**，所以 `constexpr std::vector v = ...;` 这种跨语句持有会报 C2131；惯用法是编译期函数内部消化容器、只向外返回标量/`std::array`（见 `cpp26_constexpr_vector.cpp`）。
3. **CLion/CMake 认不出 VS 2026**：CMake 3.30 没有 `Visual Studio 18 2026` 生成器，默认探测还会选中 VS 2022 的 14.38。可用方案：vcvars64 + `NMake Makefiles`（或 Ninja），或升级 CMake ≥ 4.x / 使用新版 CLion 自带 CMake。

## CI 兼容性提醒

`.github/workflows/` 目前只覆盖 GCC/Clang（Ubuntu/macOS）。上表特性（尤其 MSVC 独有节奏的 contracts）在 GCC 14/Clang 19 的支持进度不同，**`cmake/cpp26/` 暂不加入 CI 矩阵**，本地用 VS 2026 开发即可；后续可加 `windows-latest + VS 2026` runner。

## 学习路径建议（C++26 部分）

1. 先跑通 `cpp26_variadic_using`、`cpp26_structured_binding_capture`（收益最直接的日常特性）
2. 再看 `cpp26_constexpr_vector`（编译期计算范式变化）与 `cpp26_print_file`
3. 关注 MSVC 后续 18.x 更新中 contracts 求值、`#embed`、reflection 的落地（本 README ❌ 表会陆续转正）
