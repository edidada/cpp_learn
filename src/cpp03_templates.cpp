#include <iostream>

template<typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

template<typename T>
T minimum(T a, T b) {
    return (a < b) ? a : b;
}

template<typename T>
class Container {
private:
    T value;
public:
    Container(T v) : value(v) {}
    T getValue() const { return value; }
    void setValue(T v) { value = v; }
    void print() const {
        std::cout << "Container value: " << value << std::endl;
    }
};

template<typename T, int N>
class Array {
private:
    T data[N];
public:
    Array() {
        for (int i = 0; i < N; ++i) {
            data[i] = T();
        }
    }
    
    T& operator[](int index) {
        return data[index];
    }
    
    const T& operator[](int index) const {
        return data[index];
    }
    
    int size() const { return N; }
};

template<typename T1, typename T2>
class Pair {
private:
    T1 first;
    T2 second;
public:
    Pair(T1 f, T2 s) : first(f), second(s) {}
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
    void print() const {
        std::cout << "(" << first << ", " << second << ")" << std::endl;
    }
};

void test_function_templates() {
    std::cout << "=== Function Templates ===" << std::endl;
    
    std::cout << "max(3, 5) = " << maximum(3, 5) << std::endl;
    std::cout << "max(3.14, 2.71) = " << maximum(3.14, 2.71) << std::endl;
    std::cout << "max('a', 'z') = " << maximum('a', 'z') << std::endl;
    
    std::cout << "min(3, 5) = " << minimum(3, 5) << std::endl;
    std::cout << "min(3.14, 2.71) = " << minimum(3.14, 2.71) << std::endl;
}

void test_class_templates() {
    std::cout << "\n=== Class Templates ===" << std::endl;
    
    Container<int> intContainer(42);
    intContainer.print();
    
    Container<std::string> strContainer("Hello");
    strContainer.print();
    
    Container<double> doubleContainer(3.14159);
    doubleContainer.print();
}

void test_non_type_template_params() {
    std::cout << "\n=== Non-type Template Parameters ===" << std::endl;
    
    Array<int, 5> intArray;
    for (int i = 0; i < intArray.size(); ++i) {
        intArray[i] = i * 10;
    }
    
    std::cout << "Array contents: ";
    for (int i = 0; i < intArray.size(); ++i) {
        std::cout << intArray[i] << " ";
    }
    std::cout << std::endl;
    
    Array<std::string, 3> strArray;
    strArray[0] = "Hello";
    strArray[1] = "World";
    strArray[2] = "!";
    
    std::cout << "String array: ";
    for (int i = 0; i < strArray.size(); ++i) {
        std::cout << strArray[i] << " ";
    }
    std::cout << std::endl;
}

void test_multiple_template_params() {
    std::cout << "\n=== Multiple Template Parameters ===" << std::endl;
    
    Pair<int, std::string> p1(1, "one");
    p1.print();
    
    Pair<std::string, double> p2("pi", 3.14159);
    p2.print();
    
    Pair<char, int> p3('A', 65);
    p3.print();
}

int main() {
    test_function_templates();
    test_class_templates();
    test_non_type_template_params();
    test_multiple_template_params();
    return 0;
}
