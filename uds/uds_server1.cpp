// uds_server.cpp
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>

#define SOCKET_PATH "/tmp/uds_socket"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 1

int main() {
    int server_fd, client_fd;
    struct sockaddr_un address;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(address);
    int total_messages = 0;
    bool first_message_received = false;
    std::chrono::high_resolution_clock::time_point start_time, end_time;

    // 创建 UDS 套接字
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    unlink(SOCKET_PATH);  // 确保之前的套接字文件不存在

    // 绑定到 UDS 套接字文件
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on " << SOCKET_PATH << "...\n";

    // 接受客户端连接
    if ((client_fd = accept(server_fd, nullptr, nullptr)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;

    std::string message_buffer = "";

    while (true) {
        int poll_count = poll(&pfd, 1, -1);  // 无限等待数据到达
        if (poll_count < 0) {
            perror("poll");
            break;
        }

        if (pfd.revents & POLLIN) {
            memset(buffer, 0, BUFFER_SIZE);
            int valread = read(client_fd, buffer, BUFFER_SIZE);
            if (valread == 0) {
                // 客户端关闭连接
                std::cout << "Client disconnected\n";
                break;
            }

            message_buffer += std::string(buffer, valread);

            size_t pos = 0;
            while ((pos = message_buffer.find('\n')) != std::string::npos) {
                std::string message = message_buffer.substr(0, pos);
                message_buffer.erase(0, pos + 1);

                // 统计第一条消息的接收时间
                if (!first_message_received) {
                    first_message_received = true;
                    start_time = std::chrono::high_resolution_clock::now();
                }

                total_messages++;

                // 打印接收到的消息 (可选)
                // std::cout << "Message received: " << message << std::endl;
            }
        }
    }

    // 统计最后一条消息的接收时间
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    std::cout << "Total messages received: " << total_messages << "\n";
    std::cout << "Time interval (first to last message): " << elapsed_time.count() << " seconds\n";

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);  // 删除 UDS 文件
    return 0;
}
