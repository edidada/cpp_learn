#include <iostream>
#include <string>
#include <stdexcept>

class MyException : public std::exception {
private:
    std::string message;
public:
    MyException(const std::string& msg) : message(msg) {}
    virtual ~MyException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

void throw_runtime_error() {
    throw std::runtime_error("Runtime error occurred!");
}

void throw_custom_exception() {
    throw MyException("Custom exception occurred!");
}

void test_exception_specification() throw(std::runtime_error) {
    throw_runtime_error();
}

int main() {
    std::cout << "=== Exception Handling ===" << std::endl;
    
    try {
        throw_runtime_error();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error: " << e.what() << std::endl;
    }
    
    try {
        throw_custom_exception();
    } catch (const MyException& e) {
        std::cout << "Caught MyException: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught std::exception: " << e.what() << std::endl;
    }
    
    try {
        throw 42;
    } catch (int e) {
        std::cout << "Caught int: " << e << std::endl;
    }
    
    try {
        test_exception_specification();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught from exception specification: " << e.what() << std::endl;
    }
    
    return 0;
}
