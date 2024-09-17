import demo2

# Python 中继承 Shape 类并实现 area 方法
class Triangle(demo2.Shape):
    def __init__(self, base):
        self.base = base

    def area(self):
        return 0.5 * self.base

# 实例化并调用
triangle = Triangle(4)
print(f"Triangle area: {triangle.area()}")

# circle = demo2.Circle(5)
# print(f"Circle area: {circle.area()}")

# rectangle = demo2.Rectangle(3, 4)
# print(f"Rectangle area: {rectangle.area()}")
