#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <poll.h>
#include <arpa/inet.h>

#include "message.h"

const int PORT = 8080;

void handle_connection(int client_fd)
{
    Message msg;
    int bytes_received = recv(client_fd, &msg, sizeof(msg), 0);

    if (bytes_received > 0)
    {
        // 将length从网络序转换为主机序
        msg.length = ntohs(msg.length);

        // 处理接收到的消息
        std::cout << "Received message of type: " << msg.message_type
                  << std::endl;

        // 将length转换为网络序再回显给客户端
        msg.length = htons(msg.length);
        send(client_fd, &msg, sizeof(msg), 0);
    }
    else
    {
        close(client_fd);
    }
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 10);

    std::vector<struct pollfd> fds;
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    fds.push_back(server_pollfd);

    while (true)
    {
        int poll_count = poll(fds.data(), fds.size(), -1);

        if (poll_count > 0)
        {
            for (size_t i = 0; i < fds.size(); ++i)
            {
                if (fds[i].revents & POLLIN)
                {
                    if (fds[i].fd == server_fd)
                    {
                        // 新的客户端连接
                        int client_fd = accept(server_fd, NULL, NULL);
                        struct pollfd client_pollfd;
                        client_pollfd.fd = client_fd;
                        client_pollfd.events = POLLIN;
                        fds.push_back(client_pollfd);
                    }
                    else
                    {
                        // 处理已连接的客户端消息
                        handle_connection(fds[i].fd);
                        // 从poll数组中移除处理完的客户端
                        fds.erase(fds.begin() + i);
                        --i;
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
