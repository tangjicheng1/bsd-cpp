#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#define WORKER_COUNT 3

// Worker进程的功能
void workerProcess(int workerID, int read_fd) {
    char buffer[128];
    close(read_fd);  // Worker不需要读管道

    // 模拟Worker工作
    std::cout << "Worker " << workerID << " is doing work...\n";
    sleep(1 + rand() % 3);  // 模拟不同时间完成工作

    std::string result = "Result from Worker " + std::to_string(workerID) + "\n";
    write(STDOUT_FILENO, result.c_str(), result.length());  // 输出结果
    std::this_thread::sleep_for(std::chrono::seconds(5));
    exit(0);  // Worker完成任务后退出
}

// Master进程的功能
void masterProcess(const std::vector<int>& workers) {
    int status;
    for (int i = 0; i < WORKER_COUNT; i++) {
        waitpid(workers[i], &status, 0);  // 等待所有Worker进程完成
        if (WIFEXITED(status)) {
            std::cout << "Worker " << i << " finished successfully.\n";
        }
    }
}

int main() {
    std::vector<int> workers;  // 存储Worker进程的PID
    int pipefd[2];             // 管道用于master/worker通信
    pipe(pipefd);              // 创建管道

    for (int i = 0; i < WORKER_COUNT; i++) {
        int pid = fork();                 // 创建子进程
        if (pid == 0) {                   // 子进程
            workerProcess(i, pipefd[0]);  // Worker的逻辑
        } else if (pid > 0) {             // 父进程
            workers.push_back(pid);       // 保存Worker的PID
        } else {
            std::cerr << "Fork failed.\n";
            exit(1);
        }
    }

    close(pipefd[1]);        // Master不需要写管道
    masterProcess(workers);  // Master的逻辑

    return 0;
}
