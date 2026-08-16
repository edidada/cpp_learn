# C++11 学习示例（cpp11 分支）

> 本分支对应 git 分支 `cpp11`，汇集 C++11 标准引入的新特性代码示例，覆盖**语法新特性**、**智能指针/移动语义**、**多线程与同步**、**随机数**、**正则表达式**、**模板编程**等主题。

## 目录结构

```
src/
├── cpp11/            # C++11 新特性示例（核心）
├── cpp17/            # 少量 C++17 示例
├── cpp20/            # 少量 C++20 示例
├── cpp23/            # 少量 C++23 示例
├── AA.cpp / AA.h     # 类模板示例（AA 模板类）
├── NoThisFriendClass_* # 友元类 / 非 this 友元示例
├── cpp03_*.cpp       # C++03 基础语法示例
└── ...               # 其他基础示例
```

## 核心示例清单（src/cpp11/）

### 一、语言新特性

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp11_auto_range_for.cpp` | 基于范围的 for 循环遍历容器 | range-based for、auto 类型推导 |
| `cpp11_lambda.cpp` | Lambda 表达式全套用法 | 基本 lambda、捕获方式（值/引用/隐式）、返回类型推导、mutable、`std::function` |
| `cpp11_nullptr.cpp` | `nullptr` 空指针常量 | `nullptr` vs `NULL`、指针重载决议 |
| `cpp11_override_final.cpp` | 虚函数 `override` / `final` 关键字 | 显式覆盖、禁止继承/覆盖、编译期检查 |
| `cpp11_constexpr.cpp` | 常量表达式函数 | `constexpr` 函数、编译期计算 |
| `cpp11_delegating_constructor.cpp` | 委托构造函数 | 构造函数委托调用、避免代码重复 |
| `cpp11_containers.cpp` | 新增标准容器 | `std::array`、`std::unordered_map`、`std::forward_list` 等 |
| `cpp11_tuple.cpp` | 元组 `std::tuple` | `std::make_tuple`、`std::get`、`std::tie` |
| `cpp11_move_semantics.cpp` | 移动语义 | 右值引用 `&&`、移动构造/移动赋值、`std::move`、拷贝与移动对比 |
| `cpp11_smart_pointers.cpp` | 智能指针 | `std::unique_ptr`（独占）、`std::shared_ptr`（共享）、`std::weak_ptr`（弱引用）、`make_unique`/`make_shared`、`release`/`reset`/`get` |

### 二、多线程与同步（并发）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp11_condition_variable_main.cpp` | 条件变量实现线程等待/通知 | `std::condition_variable`、`wait`、`notify_one`/`notify_all` |
| `cpp11_condition_variable_any_main.cpp` | 可与任意锁配合的条件变量 | `std::condition_variable_any`、`unique_lock` |
| `cpp11_cv_status_main.cpp` | 条件变量等待超时状态 | `cv_status::no_timeout`/`timeout`、`wait_for`/`wait_until` |
| `cpp11_lock_guard_main.cpp` | 作用域锁 `lock_guard` | RAII 自动加锁/解锁、异常安全 |
| `cpp11_recursive_mutex_main.cpp` | 递归互斥锁 | `std::recursive_mutex`、同线程重复加锁 |
| `cpp11_thread_lock.cpp` | 多线程竞争与加锁 | 全局互斥量、`unique_lock` 与条件变量组合 |
| `cpp11_try_lock_main.cpp` | 非阻塞尝试加锁 | `try_lock`、`std::lock` 同时锁多个互斥量 |
| `cpp11_promise_main.cpp` | promise/future 生产者-消费者 | `std::promise::set_value`、`std::future::get`、线程间传值 |
| `cpp11_packaged_task_main.cpp` | 可调用对象包装为异步任务 | `std::packaged_task`、`get_future`、在线程中执行 |
| `cpp11_future_status_main.cpp` | future 等待状态查询 | `future_status`、`wait_for` 返回枚举 |
| `cpp11_future_erro_main.cpp` | future 异常处理 | `std::future_error`、`set_exception` |
| `cpp11_launch_main.cpp` | 异步启动策略 | `std::launch::async` / `deferred`、`std::async` |
| `cpp11_shared_future_main.cpp` | 可复制的 future | `std::shared_future`、多线程同时等待同一结果 |
| `cpp11_thread_local.cpp` | 线程局部存储 | `thread_local` 变量、各线程独立副本 |
| `t_local.cpp` | thread_local 与普通全局变量对比 | 对比是否加 `thread_local` 的输出差异 |
| `static_local.cpp` | 线程内静态局部变量 | 静态局部变量在并发下的行为、与 thread_local 对比 |

### 三、随机数

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp11_default_random_engine.cpp` | 默认随机引擎基础用法 | `std::default_random_engine`、`seed()` 设置种子 |
| `cpp11_random_device.cpp` | 硬件随机数发生器 | `std::random_device`、真随机种子来源 |
| `cpp11_uniform_int_distribution.cpp` | 均匀整数分布 | `std::uniform_int_distribution`、生成指定范围随机整数 |
| `cpp11_random.cpp` | 随机数库综合示例 | 引擎 + 分布组合、打乱容器 `std::shuffle` |

### 四、正则表达式

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp11_regex.cpp` | 正则表达式匹配 | `std::regex`、`regex_match`/`regex_search` |
| `cpp11_smatch.cpp` | 匹配结果对象 | `std::smatch`、捕获组 `std::ssub_match` |

### 五、模板编程与文件组织

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `MyClass.hpp` + `MyClass.tpp` | 模板类声明与实现分离 | 模板分离编译问题、`.tpp` 实现文件 |
| `main.cpp` | MyClass 模板使用示例 | 模板实例化、显式/隐式实例化（注释说明无法链接） |
| `MyClass2.hpp` + `MyClass2.inl` | 模板类 inline 实现 | `.inl` 文件组织方式 |
| `main2.cpp` | MyClass2 使用示例 | 头文件内联模板实现 |

### 六、其他

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `getline.cpp` | `getline` 与宏演示 | 按行读取、`__LINE__` 预定义宏 |
| `test_friend.cpp` | 友元函数/类 | `friend` 关键字、访问私有成员 |
| `test_suf.cpp` | 随机打乱 vector 示例 | `std::shuffle`、时间种子 |
| `cpp11_main.cpp` | 综合示例（占位/空） | — |

## 根目录其他示例

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `AA.cpp` / `AA.h` | 类模板 AA | 模板类定义与成员函数实现 |
| `NoThisFriendClass.cpp/.h` | 友元类示例 | 类之间的友元关系、非成员友元 |
| `virtual_main.cpp` | 虚函数多态 | 虚函数、虚析构、多态行为 |
| `singleton.cpp` | 单例模式 | 懒汉/饿汉单例、线程安全 |
| `mymalloc.cpp` | 自定义内存分配 | `operator new` 重载 / 内存池思路 |
| `simple_alloc_main.cpp` | 简单分配器 | 内存分配器设计 |
| `myset_b_tree.cpp` | B 树集合实现 | 数据结构、平衡树 |
| `isequal_main.cpp` | 相等比较工具 | 类型比较/模板元编程 |
| `cpp_err_handler.cpp` | 错误处理示例 | 错误码/异常处理 |
| `cpp03_*.cpp` | C++03 基础语法系列 | 算法、auto_ptr、类、容器、异常、函数对象、继承、IO、运算符重载、RTTI、static、模板 |

> 注：`src/cpp17`、`src/cpp20`、`src/cpp23` 目录中的少量示例（如 filesystem、concepts、coroutine、stacktrace 等）为早期移植的更高标准示例，完整体系见对应分支的文档。

## 编译运行

项目使用 CMake 管理，各示例对应独立的可执行目标，例如：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
# 构建单个示例
cmake --build . --target cpp11_lambda
```

## 学习路径建议

1. 先掌握语言特性：`nullptr` → `auto_range_for` → `lambda` → `constexpr` → `override/final` → `delegating_constructor`
2. 再学资源管理：`smart_pointers` → `move_semantics`
3. 进阶并发：`lock_guard` → `condition_variable` → `promise/future` → `packaged_task` → `async`
4. 最后看库组件：随机数、正则、`tuple`、`chrono`
