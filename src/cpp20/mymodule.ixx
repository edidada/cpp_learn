// mymodule.ixx
#include <iostream>

export module mymodule;

export void hello() {
    std::cout << "Hello from module!" << std::endl;
}