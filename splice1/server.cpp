#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // 读取客户端发送的文件名
    bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        perror("read");
        close(client_fd);
        return;
    }

    // 添加字符串结束符并去掉换行符
    buffer[bytes_read] = '\0';
    std::string filename(buffer);
    if (!filename.empty() && filename[filename.size() - 1] == '\n') {
        filename.pop_back();  // 去掉换行符
    }

    // 打开文件
    int file_fd = open(filename.c_str(), O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        close(client_fd);
        return;
    }

    // 创建管道
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        close(file_fd);
        close(client_fd);
        return;
    }

    // 使用 splice 将文件内容写入管道
    ssize_t bytes_spliced =
        splice(file_fd, NULL, pipe_fd[1], NULL, BUFFER_SIZE, SPLICE_F_MOVE | SPLICE_F_NONBLOCK);
    if (bytes_spliced < 0) {
        perror("splice");
        close(file_fd);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(client_fd);
        return;
    }

    // 使用 splice 将管道内容发送到客户端
    ssize_t bytes_sent =
        splice(pipe_fd[0], NULL, client_fd, NULL, bytes_spliced, SPLICE_F_MOVE | SPLICE_F_NONBLOCK);
    if (bytes_sent < 0) {
        perror("splice to client");
    }

    // 打印文件内容到标准输出
    // lseek(file_fd, 0, SEEK_SET);  // 重置文件描述符位置
    // char out_buffer[BUFFER_SIZE];
    // ssize_t bytes_read_file;
    // while ((bytes_read_file = read(file_fd, out_buffer, sizeof(out_buffer))) > 0) {
    //     write(STDOUT_FILENO, out_buffer, bytes_read_file);
    // }

    // 关闭文件和管道
    close(file_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    close(client_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 创建套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 绑定套接字
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // 监听连接
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // 使用 epoll 进行事件监听
    int epoll_fd = epoll_create1(0);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    while (true) {
        struct epoll_event events[MAX_EVENTS];
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_fd) {
                // 接受客户端连接
                client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (client_fd < 0) {
                    perror("accept");
                    continue;
                }
                std::cout << "Client connected" << std::endl;

                // 处理客户端请求
                handle_client(client_fd);
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
