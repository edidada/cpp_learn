# C++17 学习示例（cpp17 分支）

> 本分支对应 git 分支 `cpp17`，汇集 C++17 标准新特性代码示例，覆盖 **结构化绑定**、**折叠表达式**、**if constexpr**、**模板库组件（any/optional/variant）**、**字符串视图**、**文件系统**、**并行算法** 等主题。

## 核心示例清单（src/cpp17/）

### 一、语言新特性

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp17_structured_bindings.cpp` | 结构化绑定 | 解构 `pair`/`tuple`/`array`/`map`，`auto [a, b, c]` 语法 |
| `cpp17_fold_expressions.cpp` | 折叠表达式全套 | 一元/二元折叠、`(args + ...)`、`(std::cout << ... << args)`、带初值的折叠 |
| `cpp17_arg.cpp` | 变参模板折叠求和 | `(args + ...)` 折叠、`template<typename... Args>` |
| `cpp17_if_constexpr.cpp` | 编译期条件分支 | `if constexpr`、`std::is_integral_v` 等类型特征、编译期丢弃分支 |
| `cpp17_sfinae_main.cpp` | constexpr 与 SFINAE 结合 | 整型直接相加、自定义类型调用 `toDouble()`、编译期类型分派 |
| `cpp17_rvo.cpp` | 返回值优化 | 保证性复制省略（C++17 强制 RVO）、NRVO 具名返回值优化、构造/析构日志 |

### 二、模板库组件（utility）

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp17_any.cpp` | `std::any` 基础 | 任意类型存储、`any_cast` 安全转换 |
| `cpp17_any_detailed.cpp` | `std::any` 详细 | `has_value`、`reset`、类型不符时的异常、`type()` 查询 |
| `cpp17_optional.cpp` | `std::optional` 基础 | 可选值语义、`has_value`、`value_or` 默认值 |
| `cpp17_optional_detailed.cpp` | `std::optional` 详细 | 空 optional、`operator*`/`operator->`、`emplace`、比较运算 |
| `cpp17_variant.cpp` | `std::variant` 基础 | 类型安全联合、`std::get`、`std::holds_alternative` |
| `cpp17_variant_detailed.cpp` | `std::variant` 详细 | `get_if`、`visit` 访问、`index()`、monostate |
| `cpp17_apply.cpp` | `std::apply` | 将 tuple 展开为函数实参、与泛型 lambda 配合 |
| `cpp17_invoke.cpp` | `std::invoke` | 统一调用普通函数/成员函数/静态函数/可调用对象 |

### 三、字符串与转换

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp17_string_view.cpp` | `std::string_view` 基础 | 零拷贝字符串视图、`substr`、`find`、`remove_prefix/suffix` |
| `cpp17_string_view_detailed.cpp` | `std::string_view` 详细 | 与 `std::string` 互转、遍历、边界操作、空视图处理 |
| `cpp17_charconv.cpp` | `std::from_chars` 字符串转数值 | 无异常快速转换、`errc` 错误码、`result.ptr` 定位 |
| `cpp17_charconv2.cpp` | `from_chars` 整数转换 | `from_chars` 基本用法、错误检测 |

### 四、文件与 IO

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp17_filesystem.cpp` | 文件系统库 | `std::filesystem` 路径操作、目录遍历、文件状态查询 |
| `cpp17_fstream.cpp` | 文件流读写 | `std::ifstream`/`std::ofstream`、`getline`、写入覆盖/追加 |
| `cpp17_ini.cpp` | INI 文件解析器 | `getline` 逐行解析、`[section]` 段、`key=value`、注释过滤、`unordered_map` 存储 |

### 五、容器与算法

| 文件 | 功能说明 | 知识点 |
| --- | --- | --- |
| `cpp17_tuple.cpp` | tuple 高级用法 | `make_tuple`、`std::get`、结构化绑定解构 tuple |
| `cpp17_parallel_algorithms.cpp` | 并行算法 | 标准算法串行用法（`for_each`/`sort`/`count`/`accumulate`）、执行策略扩展 |

## 编译运行

项目使用 CMake 管理，各示例对应独立的可执行目标，例如：

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target cpp17_structured_bindings
```

## 学习路径建议

1. 语言特性：`cpp17_structured_bindings` → `cpp17_if_constexpr` → `cpp17_fold_expressions` → `cpp17_rvo`
2. 模板组件：`cpp17_any` → `cpp17_optional` → `cpp17_variant` → `cpp17_apply` → `cpp17_invoke`
3. 字符串：`cpp17_string_view` → `cpp17_charconv`
4. 实际应用：`cpp17_filesystem` → `cpp17_ini`（解析配置文件是综合练习）
