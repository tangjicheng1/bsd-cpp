#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <poll.h>
#include <arpa/inet.h>
#include <chrono>
#include "message.h"

const int PORT = 8080;
const int HEARTBEAT_INTERVAL = 1000; // 心跳间隔为 1 秒（1000 毫秒）

void handle_connection(int client_fd, bool& send_heartbeat)
{
    Message msg;
    int bytes_received = recv(client_fd, &msg, sizeof(msg), 0);

    if (bytes_received > 0)
    {
        // 将length从网络序转换为主机序
        msg.length = ntohs(msg.length);

        // 处理接收到的消息
        if (msg.message_type == MESSAGE_TYPE_HELLO)
        {
            std::cout << "Received HELLO from client." << std::endl;
            // 回复 HELLO 消息
            Message reply;
            reply.length = htons(sizeof(reply));
            reply.message_type = MESSAGE_TYPE_HELLO;
            strcpy(reply.body, "hello, client");
            send(client_fd, &reply, sizeof(reply), 0);
        }
        else if (msg.message_type == MESSAGE_TYPE_HEARTBEAT)
        {
            std::cout << "Received HEARTBEAT from client." << std::endl;
            send_heartbeat = true; // 客户端已发送心跳，准备发送心跳回去
        }
    }
    else
    {
        close(client_fd);
    }
}

void send_heartbeat_message(int client_fd)
{
    Message heartbeat;
    heartbeat.length = htons(sizeof(heartbeat));
    heartbeat.message_type = MESSAGE_TYPE_HEARTBEAT;
    strcpy(heartbeat.body, "heartbeat from server");
    send(client_fd, &heartbeat, sizeof(heartbeat), 0);
    std::cout << "Sent HEARTBEAT to client." << std::endl;
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

    bool send_heartbeat = false;
    auto last_heartbeat_time = std::chrono::steady_clock::now();

    while (true)
    {
        int poll_count = poll(fds.data(), fds.size(), HEARTBEAT_INTERVAL);

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
                        handle_connection(fds[i].fd, send_heartbeat);
                    }
                }
            }
        }

        // 发送心跳消息
        auto now = std::chrono::steady_clock::now();
        if (send_heartbeat ||
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - last_heartbeat_time)
                    .count() >= HEARTBEAT_INTERVAL)
        {
            for (size_t i = 1; i < fds.size(); ++i)
            { // 跳过 server_fd (fds[0])
                send_heartbeat_message(fds[i].fd); // 使用新的函数名
            }
            last_heartbeat_time = now;
            send_heartbeat = false;
        }
    }

    close(server_fd);
    return 0;
}
