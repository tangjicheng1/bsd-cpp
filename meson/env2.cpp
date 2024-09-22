#include <iostream>

int main(int argc, char** argv, char** envp) {
    std::cout << "命令行参数:\n";
    for (int i = 0; i < argc; i++) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }

    std::cout << "\n环境变量:\n";
    for (char** env = envp; *env != 0; env++) {
        std::cout << *env << std::endl;
    }

    std::cout << "C++标准版本: " << __cplusplus << std::endl;

    return 0;
}
