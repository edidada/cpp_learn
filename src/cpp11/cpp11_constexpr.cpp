#include <iostream>

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

constexpr int square(int x) {
    return x * x;
}

constexpr int cube(int x) {
    return x * x * x;
}

class Point {
public:
    constexpr Point(double x, double y) : x_(x), y_(y) {}
    
    constexpr double getX() const { return x_; }
    constexpr double getY() const { return y_; }
    
    constexpr double distanceSquared() const {
        return x_ * x_ + y_ * y_;
    }
    
private:
    double x_;
    double y_;
};

void test_constexpr_function() {
    std::cout << "=== constexpr Function ===" << std::endl;
    
    constexpr int fact5 = factorial(5);
    std::cout << "factorial(5) = " << fact5 << std::endl;
    
    constexpr int fib10 = fibonacci(10);
    std::cout << "fibonacci(10) = " << fib10 << std::endl;
    
    int n = 5;
    int result = factorial(n);
    std::cout << "factorial(" << n << ") at runtime = " << result << std::endl;
}

void test_constexpr_variable() {
    std::cout << "\n=== constexpr Variable ===" << std::endl;
    
    constexpr int max_size = 100;
    constexpr double pi = 3.14159265358979;
    constexpr int arr_size = square(5);
    
    int arr[arr_size];
    for (int i = 0; i < arr_size; ++i) {
        arr[i] = i;
    }
    
    std::cout << "max_size: " << max_size << std::endl;
    std::cout << "pi: " << pi << std::endl;
    std::cout << "arr_size: " << arr_size << std::endl;
    std::cout << "arr[24]: " << arr[24] << std::endl;
}

void test_constexpr_class() {
    std::cout << "\n=== constexpr Class ===" << std::endl;
    
    constexpr Point p1(3.0, 4.0);
    constexpr double x = p1.getX();
    constexpr double y = p1.getY();
    constexpr double distSq = p1.distanceSquared();
    
    std::cout << "Point(" << x << ", " << y << ")" << std::endl;
    std::cout << "Distance squared: " << distSq << std::endl;
    
    Point p2(5.0, 12.0);
    std::cout << "Runtime point distance squared: " << p2.distanceSquared() << std::endl;
}

constexpr int array_sum(const int* arr, int size) {
    return size == 0 ? 0 : arr[0] + array_sum(arr + 1, size - 1);
}

void test_constexpr_loop() {
    std::cout << "\n=== constexpr with Loop (C++14) ===" << std::endl;
    
    constexpr int arr[] = {1, 2, 3, 4, 5};
    constexpr int sum = array_sum(arr, 5);
    
    std::cout << "Array sum: " << sum << std::endl;
    std::cout << "Note: Loops in constexpr are C++14 feature" << std::endl;
}

void test_constexpr_vs_const() {
    std::cout << "\n=== constexpr vs const ===" << std::endl;
    
    int x = 5;
    const int cx = x;
    constexpr int ce = 5;
    
    std::cout << "const int cx = x; // cx is const but not constexpr" << std::endl;
    std::cout << "constexpr int ce = 5; // ce is compile-time constant" << std::endl;
    
    int arr1[ce];
    std::cout << "Array size from constexpr: " << ce << std::endl;
    
    const int size = 10;
    int arr2[size];
    std::cout << "Array size from const: " << size << std::endl;
}

template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

void test_constexpr_template() {
    std::cout << "\n=== constexpr with Template ===" << std::endl;
    
    std::cout << "Factorial<5>::value = " << Factorial<5>::value << std::endl;
    std::cout << "Factorial<10>::value = " << Factorial<10>::value << std::endl;
    
    int arr[Factorial<5>::value];
    std::cout << "Array size from Factorial<5>: " << sizeof(arr) / sizeof(arr[0]) << std::endl;
}

int main() {
    test_constexpr_function();
    test_constexpr_variable();
    test_constexpr_class();
    test_constexpr_loop();
    test_constexpr_vs_const();
    test_constexpr_template();
    return 0;
}
