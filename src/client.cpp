#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <poll.h>
#include <chrono>
#include "message.h"

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";
const int HEARTBEAT_INTERVAL = 1000; // 心跳间隔为 1 秒（1000 毫秒）

void send_heartbeat(int client_fd)
{
    Message heartbeat;
    heartbeat.length = htons(sizeof(heartbeat));
    heartbeat.message_type = MESSAGE_TYPE_HEARTBEAT;
    strcpy(heartbeat.body, "heartbeat from client");
    send(client_fd, &heartbeat, sizeof(heartbeat), 0);
    std::cout << "Sent HEARTBEAT to server." << std::endl;
}

int main()
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    struct pollfd fds[1];
    fds[0].fd = client_fd;
    fds[0].events = POLLIN;

    // 发送 HELLO 消息到服务器
    Message msg;
    msg.length = htons(sizeof(Message));
    msg.message_type = MESSAGE_TYPE_HELLO;
    strcpy(msg.body, "hello, server");
    send(client_fd, &msg, sizeof(msg), 0);

    auto last_heartbeat_time = std::chrono::steady_clock::now();

    while (true)
    {
        int poll_count = poll(fds, 1, HEARTBEAT_INTERVAL);

        if (poll_count > 0 && (fds[0].revents & POLLIN))
        {
            // 处理来自服务器的消息
            Message response;
            int bytes_received =
                recv(client_fd, &response, sizeof(response), 0);

            if (bytes_received > 0)
            {
                // 将length从网络序转换为主机序
                response.length = ntohs(response.length);

                if (response.message_type == MESSAGE_TYPE_HELLO)
                {
                    std::cout << "Received HELLO from server: " << response.body
                              << std::endl;
                }
                else if (response.message_type == MESSAGE_TYPE_HEARTBEAT)
                {
                    std::cout << "Received HEARTBEAT from server." << std::endl;
                }
            }
            else
            {
                std::cout << "Server closed the connection." << std::endl;
                break;
            }
        }

        // 发送心跳消息
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                now - last_heartbeat_time)
                .count() >= HEARTBEAT_INTERVAL)
        {
            send_heartbeat(client_fd);
            last_heartbeat_time = now;
        }
    }

    close(client_fd);
    return 0;
}
