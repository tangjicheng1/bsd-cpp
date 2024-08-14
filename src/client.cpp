#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "message.h"

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

int main()
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    Message msg;
    msg.length = sizeof(Message);
    msg.message_type = 'A';
    strcpy(msg.body, "Hello, Server! This is a message with a longer body.");

    // 将length转换为网络序
    msg.length = htons(msg.length);
    send(client_fd, &msg, sizeof(msg), 0);

    Message response;
    recv(client_fd, &response, sizeof(response), 0);

    // 将length从网络序转换为主机序
    response.length = ntohs(response.length);
    std::cout << "Received echo: " << response.body << std::endl;

    close(client_fd);
    return 0;
}
