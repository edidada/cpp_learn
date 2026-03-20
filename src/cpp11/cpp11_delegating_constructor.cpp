#include <iostream>
#include <string>

class Widget {
public:
    Widget() : id_(0), name_("default") {
        std::cout << "Default constructor" << std::endl;
    }
    
    Widget(int id) : id_(id), name_("unnamed") {
        std::cout << "Constructor with id: " << id << std::endl;
    }
    
    Widget(int id, const std::string& name) : id_(id), name_(name) {
        std::cout << "Constructor with id and name: " << id << ", " << name << std::endl;
    }
    
    Widget(const Widget& other) : id_(other.id_), name_(other.name_) {
        std::cout << "Copy constructor" << std::endl;
    }
    
    Widget(int id, const std::string& name, double value) 
        : Widget(id, name) {
        value_ = value;
        std::cout << "Delegating to Widget(id, name), then setting value: " << value << std::endl;
    }
    
    void print() const {
        std::cout << "Widget(id=" << id_ << ", name=" << name_ 
                  << ", value=" << value_ << ")" << std::endl;
    }
    
private:
    int id_;
    std::string name_;
    double value_ = 0.0;
};

void test_delegating_constructor() {
    std::cout << "=== Delegating Constructor ===" << std::endl;
    
    Widget w1;
    w1.print();
    
    Widget w2(1);
    w2.print();
    
    Widget w3(2, "test");
    w3.print();
    
    Widget w4(3, "delegated", 3.14);
    w4.print();
}

class Point {
public:
    Point() : Point(0.0, 0.0) {
        std::cout << "Default Point" << std::endl;
    }
    
    Point(double x) : Point(x, 0.0) {
        std::cout << "Point with x only" << std::endl;
    }
    
    Point(double x, double y) : x_(x), y_(y) {
        std::cout << "Point with x and y" << std::endl;
    }
    
    void print() const {
        std::cout << "Point(" << x_ << ", " << y_ << ")" << std::endl;
    }
    
private:
    double x_;
    double y_;
};

void test_chained_delegation() {
    std::cout << "\n=== Chained Delegation ===" << std::endl;
    
    Point p1;
    p1.print();
    
    Point p2(5.0);
    p2.print();
    
    Point p3(3.0, 4.0);
    p3.print();
}

int main() {
    test_delegating_constructor();
    test_chained_delegation();
    return 0;
}
