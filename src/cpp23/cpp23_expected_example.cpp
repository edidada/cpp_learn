#include <iostream>
#include <variant>
#include <string>

enum class Error { Failed };

template<typename T, typename E>
class Expected {
public:
    Expected(T value) : data_(std::move(value)), has_value_(true) {}
    Expected(E error) : data_(std::move(error)), has_value_(false) {}
    
    bool has_value() const { return has_value_; }
    operator bool() const { return has_value_; }
    
    T& operator*() { return std::get<T>(data_); }
    const T& operator*() const { return std::get<T>(data_); }
    
    E& error() { return std::get<E>(data_); }
    const E& error() const { return std::get<E>(data_); }
    
private:
    std::variant<T, E> data_;
    bool has_value_;
};

Expected<int, Error> getValue(bool shouldFail) {
    if (shouldFail) {
        return Error::Failed;
    }
    return 42;
}

int main() {
    auto result = getValue(false);
    if (result) {
        std::cout << "Value: " << *result << std::endl;
    } else {
        std::cout << "Error: " << static_cast<int>(result.error()) << std::endl;
    }
    
    std::cout << "Note: <expected> is C++23 feature, using variant-based implementation" << std::endl;
    return 0;
}
