#include <iostream>
#include <string>

class MyClass {
private:
    int privateVar;
    
    void privateMethod() {
        std::cout << "Private method called" << std::endl;
    }
    
protected:
    int protectedVar;
    
    void protectedMethod() {
        std::cout << "Protected method called" << std::endl;
    }
    
public:
    int publicVar;
    
    MyClass(int pv, int pr, int pub) 
        : privateVar(pv), protectedVar(pr), publicVar(pub) {}
    
    void publicMethod() {
        std::cout << "Public method called" << std::endl;
        privateMethod();
    }
    
    int getPrivateVar() const { return privateVar; }
    int getProtectedVar() const { return protectedVar; }
};

class Derived : public MyClass {
public:
    Derived(int pv, int pr, int pub) : MyClass(pv, pr, pub) {}
    
    void accessProtected() {
        std::cout << "Accessing protected from derived: " << protectedVar << std::endl;
        protectedMethod();
    }
};

class Point {
private:
    int x, y;
    
public:
    Point() : x(0), y(0) {
        std::cout << "Default constructor" << std::endl;
    }
    
    Point(int xVal, int yVal) : x(xVal), y(yVal) {
        std::cout << "Parameterized constructor: (" << x << ", " << y << ")" << std::endl;
    }
    
    Point(const Point& other) : x(other.x), y(other.y) {
        std::cout << "Copy constructor: (" << x << ", " << y << ")" << std::endl;
    }
    
    ~Point() {
        std::cout << "Destructor: (" << x << ", " << y << ")" << std::endl;
    }
    
    Point& operator=(const Point& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            std::cout << "Assignment operator: (" << x << ", " << y << ")" << std::endl;
        }
        return *this;
    }
    
    int getX() const { return x; }
    int getY() const { return y; }
    
    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }
    
    void print() const {
        std::cout << "Point(" << x << ", " << y << ")" << std::endl;
    }
};

class Rectangle {
private:
    Point topLeft;
    Point bottomRight;
    
public:
    Rectangle(int x1, int y1, int x2, int y2) 
        : topLeft(x1, y1), bottomRight(x2, y2) {
        std::cout << "Rectangle created" << std::endl;
    }
    
    void print() const {
        std::cout << "Rectangle: ";
        topLeft.print();
        std::cout << "          ";
        bottomRight.print();
    }
};

void test_access_specifiers() {
    std::cout << "=== Access Specifiers ===" << std::endl;
    
    MyClass obj(1, 2, 3);
    
    std::cout << "Public var: " << obj.publicVar << std::endl;
    obj.publicMethod();
    
    Derived d(10, 20, 30);
    d.accessProtected();
}

void test_constructors() {
    std::cout << "\n=== Constructors and Destructors ===" << std::endl;
    
    Point p1;
    Point p2(3, 4);
    Point p3 = p2;
    Point p4;
    p4 = p1;
    
    p1.print();
    p2.print();
    p3.print();
    p4.print();
}

void test_member_initialization() {
    std::cout << "\n=== Member Initialization List ===" << std::endl;
    
    Rectangle r(0, 0, 10, 20);
    r.print();
}

int main() {
    test_access_specifiers();
    test_constructors();
    test_member_initialization();
    return 0;
}
