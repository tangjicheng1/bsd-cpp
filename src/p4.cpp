#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include <iostream>
#include <thread>

void start_worker() {
    // 设置当父进程退出时，子进程接收SIGTERM信号
    prctl(PR_SET_PDEATHSIG, SIGTERM);

    // 如果父进程在子进程启动后退出，这里可以检测到并终止
    pid_t ppid = getppid();
    if (ppid == 1) {
        // 父进程已经退出，子进程也退出
        std::cout << "Parent process has already exited. Child exiting." << std::endl;
        exit(1);
    }

    // 子进程的实际工作逻辑
    while (true) {
        std::cout << "Worker process " << getpid() << " is running..." << std::endl;
        sleep(2);  // 模拟一些工作
    }
}

int main() {
    const int num_workers = 4;  // 启动4个worker进程

    std::cout << "parent process " << getpid() << " is running..." << std::endl;

    for (int i = 0; i < num_workers; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << "Failed to fork process" << std::endl;
            return 1;
        } else if (pid == 0) {
            // 子进程
            start_worker();
            return 0;  // 结束子进程
        }
        // 父进程继续循环创建子进程
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));  // 父进程休眠10秒

    // 父进程等待（这里可以根据需求处理其他工作或者监控子进程）
    for (int i = 0; i < num_workers; ++i) {
        wait(NULL);  // 等待所有子进程退出
    }

    std::cout << "All workers have exited. Parent process exiting." << std::endl;
    return 0;
}
