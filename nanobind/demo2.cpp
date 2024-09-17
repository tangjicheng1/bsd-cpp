#include <nanobind/nanobind.h>
#include <cmath>

namespace nb = nanobind;

// 定义抽象类 Shape
class Shape {
   public:
    virtual ~Shape() = default;

    // 纯虚函数，必须由子类实现
    virtual double area() const = 0;
};

// 定义派生类 Circle
class Circle : public Shape {
   public:
    Circle(double radius) : radius_(radius) {
    }

    double area() const override {
        return M_PI * radius_ * radius_;
    }

   private:
    double radius_;
};

// 定义派生类 Rectangle
class Rectangle : public Shape {
   public:
    Rectangle(double width, double height) : width_(width), height_(height) {
    }

    double area() const override {
        return width_ * height_;
    }

   private:
    double width_;
    double height_;
};

// 使用 nanobind 绑定类到 Python
NB_MODULE(demo2, m) {
    // 绑定抽象类 Shape
    nb::class_<Shape>(m, "Shape").def("area", &Shape::area);

    // 绑定派生类 Circle
    nb::class_<Circle, Shape>(m, "Circle")
        .def(nb::init<double>())  // 构造函数
        .def("area", &Circle::area);

    // 绑定派生类 Rectangle
    nb::class_<Rectangle, Shape>(m, "Rectangle")
        .def(nb::init<double, double>())  // 构造函数
        .def("area", &Rectangle::area);
}
