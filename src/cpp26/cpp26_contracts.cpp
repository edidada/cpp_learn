#include <print>

// C++26 特性：契约式编程 (Contracts, P2900) —— MSVC 14.51 实测现状
//
// 现状（VS 2026 18.10 stable 实测）：
//   ✔ 接受 MSVC 早期括号形式：[[pre(expr)]] [[contract_assert(expr)]]
//   ✔ 接受评估模式属性：[[contract_semantics]]（承诺任何构建配置下都求值）
//                     [[contract_properties]] / [[contract_built_in]] 亦可解析
//   ✘ 尚不识别 ISO 定稿的冒号语法 [[pre: expr]]
//   ✘ 18.10 stable 实测：即使标注 contract_semantics，违反时也不会终止进程
//     —— 即"可解析、暂不强制执行"，求值语义待后续版本对齐。

// 前置条件：调用方对入参的承诺
[[contract_semantics]]
int half(int n) [[pre(n >= 0)]] {
    return n / 2;
}

// 函数体内的中间状态承诺
void print_first(const int* p, int n) {
    [[contract_assert(p != nullptr)]];
    [[contract_assert(n > 0)]];
    std::println("first = {}, (n={})", *p, n);
}

int main() {
    std::println("half(10) = {}", half(10));
    const int data[3] = {7, 8, 9};
    print_first(data, 3);
    // 待 MSVC 启用求值后，下面这行会触发契约违反 -> 默认 terminate：
    // half(-1);
    return 0;
}
