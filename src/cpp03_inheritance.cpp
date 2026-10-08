#include <iostream>
#include <string>

class Shape {
protected:
    std::string name;
public:
    Shape(const std::string& n) : name(n) {
        std::cout << "Shape constructor: " << name << std::endl;
    }
    virtual ~Shape() {
        std::cout << "Shape destructor: " << name << std::endl;
    }
    virtual void draw() const = 0;
    virtual double area() const = 0;
    std::string getName() const { return name; }
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : Shape("Circle"), radius(r) {
        std::cout << "Circle constructor, radius = " << radius << std::endl;
    }
    ~Circle() {
        std::cout << "Circle destructor" << std::endl;
    }
    void draw() const {
        std::cout << "Drawing Circle with radius " << radius << std::endl;
    }
    double area() const {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;
public:
    Rectangle(double w, double h) : Shape("Rectangle"), width(w), height(h) {
        std::cout << "Rectangle constructor, " << width << " x " << height << std::endl;
    }
    ~Rectangle() {
        std::cout << "Rectangle destructor" << std::endl;
    }
    void draw() const {
        std::cout << "Drawing Rectangle " << width << " x " << height << std::endl;
    }
    double area() const {
        return width * height;
    }
};

class Square : public Rectangle {
public:
    Square(double side) : Rectangle(side, side) {
        std::cout << "Square constructor, side = " << side << std::endl;
    }
    ~Square() {
        std::cout << "Square destructor" << std::endl;
    }
    void draw() const {
        std::cout << "Drawing Square" << std::endl;
    }
};

void test_inheritance() {
    std::cout << "=== Inheritance ===" << std::endl;
    
    Circle c(5.0);
    c.draw();
    std::cout << "Area: " << c.area() << std::endl;
    
    std::cout << std::endl;
    
    Rectangle r(3.0, 4.0);
    r.draw();
    std::cout << "Area: " << r.area() << std::endl;
    
    std::cout << std::endl;
    
    Square s(2.0);
    s.draw();
    std::cout << "Area: " << s.area() << std::endl;
}

void test_polymorphism() {
    std::cout << "\n=== Polymorphism ===" << std::endl;
    
    Shape* shapes[3];
    shapes[0] = new Circle(5.0);
    shapes[1] = new Rectangle(3.0, 4.0);
    shapes[2] = new Square(2.0);
    
    for (int i = 0; i < 3; ++i) {
        shapes[i]->draw();
        std::cout << "Area: " << shapes[i]->area() << std::endl;
        delete shapes[i];
        std::cout << std::endl;
    }
}

int main() {
    test_inheritance();
    test_polymorphism();
    return 0;
}
