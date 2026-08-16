#include <iostream>

class Point {
private:
    int x, y;
    
public:
    Point(int xVal = 0, int yVal = 0) : x(xVal), y(yVal) {}
    
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    
    Point operator*(int scalar) const {
        return Point(x * scalar, y * scalar);
    }
    
    Point operator/(int scalar) const {
        return Point(x / scalar, y / scalar);
    }
    
    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    int& operator[](int index) {
        if (index == 0) return x;
        return y;
    }
    
    const int& operator[](int index) const {
        if (index == 0) return x;
        return y;
    }
    
    Point operator-() const {
        return Point(-x, -y);
    }
    
    Point& operator++() {
        ++x;
        ++y;
        return *this;
    }
    
    Point operator++(int) {
        Point temp = *this;
        ++x;
        ++y;
        return temp;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    friend std::istream& operator>>(std::istream& is, Point& p);
    
    void print() const {
        std::cout << "Point(" << x << ", " << y << ")" << std::endl;
    }
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
}

Point operator*(int scalar, const Point& p) {
    return p * scalar;
}

void test_arithmetic_operators() {
    std::cout << "=== Arithmetic Operators ===" << std::endl;
    
    Point p1(3, 4);
    Point p2(1, 2);
    
    Point p3 = p1 + p2;
    std::cout << "p1 + p2 = " << p3 << std::endl;
    
    Point p4 = p1 - p2;
    std::cout << "p1 - p2 = " << p4 << std::endl;
    
    Point p5 = p1 * 2;
    std::cout << "p1 * 2 = " << p5 << std::endl;
    
    Point p6 = 3 * p1;
    std::cout << "3 * p1 = " << p6 << std::endl;
}

void test_comparison_operators() {
    std::cout << "\n=== Comparison Operators ===" << std::endl;
    
    Point p1(3, 4);
    Point p2(3, 4);
    Point p3(5, 6);
    
    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    std::cout << "p1 != p3: " << (p1 != p3) << std::endl;
    std::cout << "p1 < p3: " << (p1 < p3) << std::endl;
}

void test_subscript_operator() {
    std::cout << "\n=== Subscript Operator ===" << std::endl;
    
    Point p(10, 20);
    std::cout << "p[0] = " << p[0] << std::endl;
    std::cout << "p[1] = " << p[1] << std::endl;
    
    p[0] = 100;
    p[1] = 200;
    std::cout << "After modification: " << p << std::endl;
}

void test_unary_operators() {
    std::cout << "\n=== Unary Operators ===" << std::endl;
    
    Point p(3, 4);
    Point negP = -p;
    std::cout << "-p = " << negP << std::endl;
    
    Point p2(1, 1);
    std::cout << "Original: " << p2 << std::endl;
    ++p2;
    std::cout << "After ++p: " << p2 << std::endl;
    p2++;
    std::cout << "After p++: " << p2 << std::endl;
}

void test_io_operators() {
    std::cout << "\n=== I/O Operators ===" << std::endl;
    
    Point p(5, 10);
    std::cout << "Output: " << p << std::endl;
}

int main() {
    test_arithmetic_operators();
    test_comparison_operators();
    test_subscript_operator();
    test_unary_operators();
    test_io_operators();
    return 0;
}
