#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

struct Square {
    int operator()(int x) const {
        return x * x;
    }
};

struct Add {
    int operator()(int a, int b) const {
        return a + b;
    }
};

struct Print {
    void operator()(int x) const {
        std::cout << x << " ";
    }
};

bool isEven(int x) {
    return x % 2 == 0;
}

class GreaterThan {
    int threshold;
public:
    GreaterThan(int t) : threshold(t) {}
    bool operator()(int x) const {
        return x > threshold;
    }
};

void test_function_objects() {
    std::cout << "=== Function Objects ===" << std::endl;
    
    Square square;
    std::cout << "Square of 5: " << square(5) << std::endl;
    
    Add add;
    std::cout << "3 + 4 = " << add(3, 4) << std::endl;
    
    Print print;
    std::cout << "Print: ";
    print(1);
    print(2);
    print(3);
    std::cout << std::endl;
}

void test_std_function_objects() {
    std::cout << "\n=== Standard Function Objects ===" << std::endl;
    
    std::plus<int> plus;
    std::minus<int> minus;
    std::multiplies<int> multiplies;
    std::divides<int> divides;
    std::modulus<int> modulus;
    std::negate<int> negate;
    
    std::cout << "10 + 5 = " << plus(10, 5) << std::endl;
    std::cout << "10 - 5 = " << minus(10, 5) << std::endl;
    std::cout << "10 * 5 = " << multiplies(10, 5) << std::endl;
    std::cout << "10 / 5 = " << divides(10, 5) << std::endl;
    std::cout << "10 % 3 = " << modulus(10, 3) << std::endl;
    std::cout << "-10 = " << negate(10) << std::endl;
    
    std::greater<int> greater;
    std::less<int> less;
    std::equal_to<int> equal;
    
    std::cout << "10 > 5: " << greater(10, 5) << std::endl;
    std::cout << "10 < 5: " << less(10, 5) << std::endl;
    std::cout << "10 == 10: " << equal(10, 10) << std::endl;
}

void test_binders() {
    std::cout << "\n=== Binders ===" << std::endl;
    
    std::vector<int> v;
    for (int i = 1; i <= 10; ++i) {
        v.push_back(i);
    }
    
    std::cout << "Numbers: ";
    std::for_each(v.begin(), v.end(), Print());
    std::cout << std::endl;
    
    int count = std::count_if(v.begin(), v.end(), isEven);
    std::cout << "Even numbers: " << count << std::endl;
    
    GreaterThan gt5(5);
    count = std::count_if(v.begin(), v.end(), gt5);
    std::cout << "Numbers > 5: " << count << std::endl;
}

int main() {
    test_function_objects();
    test_std_function_objects();
    test_binders();
    return 0;
}
