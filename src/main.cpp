#include <iostream>

void func(double f)
{
    std::cout << f << std::endl;
    return;
}

int main()
{
    std::cout << "hello" << std::endl;
    func(1.2);
    return 0;
}