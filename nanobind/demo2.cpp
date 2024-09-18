#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>
#include <cmath>

namespace nb = nanobind;

// 定义 Shape 抽象类
class Shape {
   public:
    virtual ~Shape() = default;

    // 纯虚函数，允许 Python 覆盖
    virtual double area() const = 0;
};

// 支持 Python 覆盖 Shape 类的虚函数
class BindedShape : public Shape {
   public:
    NB_TRAMPOLINE(Shape, sizeof(Shape));

    // 覆写 area 函数，让 Python 中的类可以覆写该方法
    double area() const override {
        NB_OVERRIDE(area);
    }
};

class Circle : public Shape {
   public:
    Circle(double radius) : radius_(radius) {
    }

    // 实现 area 函数
    double area() const override {
        return M_PI * radius_ * radius_;
    }

   private:
    double radius_;
};

class Rectangle : public Shape {
   public:
    Rectangle(double width, double height) : width_(width), height_(height) {
    }

    // 实现 area 函数
    double area() const override {
        return width_ * height_;
    }

   private:
    double width_, height_;
};

// 模块定义
NB_MODULE(demo2, m) {
    // 绑定 Shape 类
    nb::class_<Shape, BindedShape>(m, "Shape").def("area", &Shape::area);

    // 绑定 Circle 类
    nb::class_<Circle, Shape>(m, "Circle")
        .def(nb::init<double>())  // 构造函数
        .def("area", &Circle::area);

    // 绑定 Rectangle 类
    nb::class_<Rectangle, Shape>(m, "Rectangle")
        .def(nb::init<double, double>())  // 构造函数
        .def("area", &Rectangle::area);
}
