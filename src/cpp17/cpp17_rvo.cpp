#include <iostream>
#include <string>

// 用构造/析构日志观察返回值优化（RVO）和具名返回值优化（NRVO）。
class Trace {
public:
    explicit Trace(std::string name) : name_(std::move(name)) {
        std::cout << "  construct  " << name_ << " @ " << this << '\n';
    }

    Trace(const Trace& other) : name_(other.name_ + " (copy)") {
        std::cout << "  copy       " << name_ << " @ " << this << '\n';
    }

    Trace(Trace&& other) noexcept : name_(std::move(other.name_)) {
        std::cout << "  move       " << name_ << " @ " << this << '\n';
    }

    ~Trace() {
        std::cout << "  destroy    " << name_ << " @ " << this << '\n';
    }

private:
    std::string name_;
};

// C++17：返回纯右值时，此处的复制省略是语言保证的。
Trace makeGuaranteedRvo() {
    return Trace{"temporary returned by value"};
}

// 返回具名局部变量：这是 NRVO，编译器通常会执行，但标准不强制。
Trace makeNrvo() {
    Trace local{"named local"};
    std::cout << "  local is        @ " << &local << '\n';
    return local;
}

int main() {
    std::cout << "Guaranteed RVO (C++17):\n";
    Trace guaranteed = makeGuaranteedRvo();
    std::cout << "  caller object is @ " << &guaranteed << "\n\n";

    std::cout << "NRVO (normally enabled):\n";
    Trace nrvo = makeNrvo();
    std::cout << "  caller object is @ " << &nrvo << '\n';

    std::cout << "\nIf NRVO is applied, the two addresses in the second section match\n"
                 "and no copy/move constructor is printed.\n";
}
