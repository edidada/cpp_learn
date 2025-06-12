#include <iostream>
#include <stacktrace>

void show_stacktrace() {
    std::cout << std::stacktrace::current();
}

int main() {
    try {
        show_stacktrace();
    } catch (...) {
        std::cerr << "Exception occurred\n";
    }
    return 0;
}