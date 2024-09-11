#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>

const char* SHM_NAME = "/my_shared_memory";
const size_t SHM_SIZE = 4096;  // 4KB

int main() {
    // 创建共享内存对象
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 设置共享内存的大小
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    // 将共享内存映射到进程地址空间
    void* ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 写入数据
    const char* message = "Hello from shared memory!";
    std::memcpy(ptr, message, std::strlen(message) + 1);

    std::cout << "Data written to shared memory: " << message << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 取消映射
    if (munmap(ptr, SHM_SIZE) == -1) {
        perror("munmap");
        return 1;
    }

    // 关闭共享内存对象
    if (close(shm_fd) == -1) {
        perror("close");
        return 1;
    }

    // 删除共享内存对象
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    return 0;
}
