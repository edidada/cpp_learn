# C++03 学习示例（cpp03 分支）

> 本分支对应 git 分支 `cpp03`。C++03 是 C++98 的**技术勘误修订版**（TC1），语言核心特性与 C++98 基本一致，主要修正标准库缺陷。本分支汇集 C++98/03 时代的基础语法完整示例，覆盖**模板**、**类/继承**、**运算符重载**、**RTTI**、**异常**、**STL 容器**、**函数对象**、**auto_ptr 智能指针**等主题，是理解"标准 C++ 起点"的最佳入口。

## 核心示例清单（src/cpp03_*.cpp）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp03_templates.cpp` | 模板全套基础 | 函数模板 `maximum/minimum`、类模板 `Container`、非类型模板参数 `Array<T,N>`、多参数模板 `Pair<T1,T2>` |
| `cpp03_class_basics.cpp` | 类的基础语法 | 构造函数/析构、成员函数、`const` 成员、访问控制 |
| `cpp03_inheritance.cpp` | 继承与多态 | 继承、虚函数、虚析构、`protected` |
| `cpp03_operators.cpp` | 运算符重载 | `operator+`、`operator<<`、`operator[]` 等常见重载 |
| `cpp03_exception.cpp` | 异常处理 | `try/catch/throw`、异常类型、`catch(...)` |
| `cpp03_rtti.cpp` | 运行时类型识别 | `typeid`、`dynamic_cast`、虚函数下的类型判断 |
| `cpp03_containers.cpp` | STL 容器 | `vector`、`map`、`string`、`list` 等基本用法 |
| `cpp03_algorithms.cpp` | STL 算法 | `sort`、`find`、`for_each`、`count` 等 |
| `cpp03_function_objects.cpp` | 函数对象 | 重载 `operator()`、`std::less/greater`、谓词 |
| `cpp03_auto_ptr.cpp` | 智能指针 `auto_ptr` | `auto_ptr` 所有权转移语义（拷贝即转移）、自定义 `AutoPtr` 实现、`release/reset/get` |
| `cpp03_iostream.cpp` | IO 流 | `cin/cout/cerr`、格式化输出、文件流 |
| `cpp03_static.cpp` | static 全场景 | 静态局部变量、静态成员变量、静态成员函数 |

> 说明：`cpp03_auto_ptr.cpp` 演示的 `std::auto_ptr` 是 C++98/03 时代唯一的"智能指针"，其**拷贝即转移所有权**的设计被证明是失败的设计，C++11 起被 `std::unique_ptr` 取代并移除。

## 编译运行

项目使用 CMake 管理，各示例对应独立的可执行目标，例如：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target cpp03_templates
```

## 学习路径建议

1. 语法基础：`cpp03_class_basics` → `cpp03_inheritance` → `cpp03_operators`
2. 泛型：`cpp03_templates`（函数模板 → 类模板 → 非类型参数）
3. 运行期能力：`cpp03_exception` → `cpp03_rtti`
4. 标准库：`cpp03_containers` → `cpp03_algorithms` → `cpp03_function_objects`
5. 资源管理：`cpp03_auto_ptr`（重点理解其缺陷，为学习 C++11 智能指针做铺垫）

---

## C++98/03 特性解决了什么问题？

C++98/03 时代确立的核心语言能力与动机：

| 特性 | 解决的问题 | C 时代/之前方案与不足 |
| --- | --- | --- |
| **模板** | 一份代码适配多种类型（类型安全的泛型） | C 用 `void*` 丢失类型、宏无类型检查；`qsort` 需函数指针且无类型安全 |
| **运算符重载** | 自定义类型使用自然表达式语法 | C 只能 `add(a,b)` 式函数调用，代码可读性差 |
| **异常** | 错误沿栈传播、自动触发析构清理 | C 的 `errno` 错误码易被忽略；`longjmp` 跳过析构，资源泄漏且不可控 |
| **RTTI** | 运行时安全识别/转换真实类型 | C 无机制，靠手写 `enum` 标志位，类型不安全 |
| **STL（容器+算法+迭代器）** | 通用数据结构与算法统一抽象 | C 每次都要手写链表/动态数组/排序，重复劳动且 bug 频发 |
| **函数对象** | 带状态的可调用对象、与算法配合 | C 用全局变量 + 函数指针，状态与逻辑分离，无法内联优化 |
| **iostream** | 类型安全 IO | `printf/%d` 类型不匹配即未定义行为 |
| **auto_ptr（C++98）** | 第一个"智能指针"，意图用析构自动释放堆资源 | 裸 `new/delete` 手动管理，异常路径泄漏；但 `auto_ptr` 拷贝转移所有权，破坏直觉且不能放标准容器 |

## 与 C / Rust 的对比

| 维度 | C | C++98/03 | Rust |
| --- | --- | --- | --- |
| **泛型** | `void*`+宏 | 模板（编译期展开，报错冗长） | 泛型 + Trait，报错清晰 |
| **错误处理** | `errno` 错误码 | 异常（未捕获即终止，开销大） | `Result<T,E>` + `?`，显式且零开销 |
| **对象能力** | 无（struct + 函数） | 类 + 继承 + 多态（动态分派） | `struct + impl` + Trait（静态/动态分派） |
| **空指针** | `NULL`（整数 0） | `NULL`（同 C） | `Option<T>`，无空指针 |
| **智能指针** | 无 | `auto_ptr`（所有权转移，缺陷多） | `Box<T>`/`Rc<T>`/`Arc<T>` 所有权体系完备 |
| **并发** | pthread 手工 | 无标准线程（第三方） | 标准库内建线程 + `Send/Sync` 编译期检查 |
| **内存安全** | 手动 | 手动为主，局部 RAII | 编译期保证，无悬垂/数据竞争 |

**一句话总结**：C++98/03 确立了 C++ 的语言骨架——**类、模板、STL、异常、RTTI**，把 C 的结构化编程升级为面向对象 + 泛型编程；但所有权模型（`auto_ptr`）、空指针（`NULL`）、并发支持上的缺陷，成为 C++11 乃至 Rust 大力改进的核心方向。
