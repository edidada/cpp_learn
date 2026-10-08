#include <cassert>

int divide(int a, int b) {
    [[assume(b != 0)]];  // 告诉编译器 b 永远不会为 0
    return a / b;
}

int main() {
    int result = divide(10, 2);  // 编译器会基于假设优化
    assert(result == 5);

    return 0;
}