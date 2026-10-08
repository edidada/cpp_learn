#include <iostream>
#include <utility>
#include <string>
#include <vector>

template<typename T, typename U = T>
T my_exchange(T& obj, U&& new_value) {
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

void test_exchange() {
    std::cout << "=== std::exchange ===" << std::endl;
    
    int x = 10;
    int old_value = std::exchange(x, 20);
    std::cout << "Old value: " << old_value << std::endl;
    std::cout << "New value: " << x << std::endl;
    
    std::string s = "Hello";
    std::string old_str = std::exchange(s, std::string("World"));
    std::cout << "Old string: " << old_str << std::endl;
    std::cout << "New string: " << s << std::endl;
}

void test_exchange_in_class() {
    std::cout << "\n=== std::exchange in Class ===" << std::endl;
    
    class Container {
    public:
        void set_data(int new_data) {
            old_data_ = std::exchange(data_, new_data);
        }
        
        int get_data() const { return data_; }
        int get_old_data() const { return old_data_; }
        
    private:
        int data_ = 0;
        int old_data_ = 0;
    };
    
    Container c;
    c.set_data(10);
    std::cout << "Data: " << c.get_data() << ", Old: " << c.get_old_data() << std::endl;
    
    c.set_data(20);
    std::cout << "Data: " << c.get_data() << ", Old: " << c.get_old_data() << std::endl;
}

void test_exchange_implementation() {
    std::cout << "\n=== exchange Implementation ===" << std::endl;
    
    int x = 100;
    int old = my_exchange(x, 200);
    std::cout << "my_exchange: old=" << old << ", new=" << x << std::endl;
}

int main() {
    test_exchange();
    test_exchange_in_class();
    test_exchange_implementation();
    return 0;
}
