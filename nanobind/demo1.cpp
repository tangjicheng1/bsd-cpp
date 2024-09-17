#include <nanobind/nanobind.h>

namespace nb = nanobind;

int add(int a, int b) {
    return a + b;
}

class Point {
   public:
    Point(double x, double y) : x_(x), y_(y) {
    }

    double get_x() const {
        return x_;
    }
    double get_y() const {
        return y_;
    }

    void set_x(double x) {
        x_ = x;
    }
    void set_y(double y) {
        y_ = y;
    }

    double distance_to(const Point& other) const {
        double dx = x_ - other.x_;
        double dy = y_ - other.y_;
        return std::sqrt(dx * dx + dy * dy);
    }

   private:
    double x_;
    double y_;
};

NB_MODULE(demo1, m) {
    m.def("add", &add);
    nb::class_<Point>(m, "Point")
        .def(nb::init<double, double>(), "Constructor with x and y")
        .def("get_x", &Point::get_x)
        .def("get_y", &Point::get_y)
        .def("set_x", &Point::set_x)
        .def("set_y", &Point::set_y)
        .def("distance_to", &Point::distance_to, "Calculate distance to another point");
}
