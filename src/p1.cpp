#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h> // for fork, getpid
#include <sys/wait.h> // for wait

int main(int argc, char* argv[]) {
    // 检查是否传递了两个参数
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <arg1> <arg2>" << std::endl;
        return 1;
    }

    // 获取参数
    std::string arg1 = argv[1];
    std::string arg2 = argv[2];

    // 创建子进程
    pid_t pid = fork();

    if (pid < 0) {
        // fork失败
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    } else if (pid == 0) {
        // 子进程
        std::cout << "Child process, PID: " << getpid() << ", arg2: " << arg2 << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(100));
    } else {
        // 父进程
        std::cout << getpid() << "Parent process, processing arg1: " << arg1 << std::endl;
        // 等待子进程结束
        std::this_thread::sleep_for(std::chrono::seconds(100));

        wait(nullptr);
    }

    return 0;
}
