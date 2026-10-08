// main.cpp
#include <iostream>
#include "MyClass.hpp"

int main() {
    MyClass<int> myInt;
    myInt.add(5);
    myInt.add(10);
    std::cout << "The value is: " << myInt.getValue() << std::endl;

    MyClass<std::string> myString;
    myString.add("Hello, ");
    myString.add("World!");
    std::cout << "The string is: " << myString.getValue() << std::endl;

    return 0;
}
