#include <fcntl.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    // 创建套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }

    // 绑定
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    // 监听
    listen(server_fd, 3);
    std::cout << "Server listening on port " << PORT << std::endl;

    // 接受客户端连接
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    if (client_fd < 0) {
        perror("accept");
        return -1;
    }

    // 打开文件
    int file_fd = open("example.txt", O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        close(client_fd);
        return -1;
    }

    // 使用 sendfile 发送文件
    off_t offset = 0;
    ssize_t sent_bytes = sendfile(client_fd, file_fd, &offset, 0);
    if (sent_bytes < 0) {
        perror("sendfile");
    } else {
        std::cout << "Sent " << sent_bytes << " bytes to client." << std::endl;
    }

    // 清理
    close(file_fd);
    close(client_fd);
    close(server_fd);
    return 0;
}
