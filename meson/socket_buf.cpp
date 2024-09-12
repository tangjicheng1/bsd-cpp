#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define PORT 8080
#define BUFFER_SIZE 512 * 1024 * 1024  // 512MB

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置端口复用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 设置socket接收缓冲区大小为512MB
    int requested_buf_size = BUFFER_SIZE;
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVBUF, &requested_buf_size,
                   sizeof(requested_buf_size)) < 0) {
        perror("setsockopt - SO_RCVBUF");
        exit(EXIT_FAILURE);
    }

    // 获取实际设置的接收缓冲区大小
    int actual_buf_size;
    socklen_t optlen = sizeof(actual_buf_size);
    if (getsockopt(server_fd, SOL_SOCKET, SO_RCVBUF, &actual_buf_size, &optlen) < 0) {
        perror("getsockopt - SO_RCVBUF");
        exit(EXIT_FAILURE);
    }

    // 打印请求的和实际的接收缓冲区大小
    std::cout << "Requested socket recv buffer size: " << requested_buf_size << " bytes"
              << std::endl;
    std::cout << "Actual socket recv buffer size: " << actual_buf_size << " bytes" << std::endl;

    // 配置服务器地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket到端口
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "TCP server is running and waiting for connections on port " << PORT << std::endl;

    // 接受连接 (示例)
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 关闭socket
    close(new_socket);
    close(server_fd);

    return 0;
}
