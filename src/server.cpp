#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include "message.h"

const int PORT = 8080;

void handle_connection(int client_fd, struct kevent& event)
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
        EV_SET(&event, client_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
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

    int kq = kqueue();
    struct kevent event;
    EV_SET(&event, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event, 1, NULL, 0, NULL);

    std::vector<struct kevent> events(10);
    while (true)
    {
        int event_count =
            kevent(kq, NULL, 0, events.data(), events.size(), NULL);
        for (int i = 0; i < event_count; ++i)
        {
            if (events[i].ident == server_fd)
            {
                int client_fd = accept(server_fd, NULL, NULL);
                EV_SET(&event, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &event, 1, NULL, 0, NULL);
            }
            else
            {
                handle_connection(events[i].ident, events[i]);
            }
        }
    }

    close(server_fd);
    return 0;
}
