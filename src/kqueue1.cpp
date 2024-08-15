#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/time.h>

#define PORT 8080
#define BACKLOG 10
#define BUFFER_SIZE 1024

void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);
    }

    if (bytes_read == -1)
    {
        perror("read");
    }

    close(client_fd);
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    int kq, nev;
    struct kevent change_event;
    struct kevent event_list[BACKLOG];

    // 创建监听套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字到端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==
        -1)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, BACKLOG) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 创建kqueue
    kq = kqueue();
    if (kq == -1)
    {
        perror("kqueue");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 将监听套接字的可读事件加入kqueue
    EV_SET(
        &change_event, server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        // 等待事件
        nev = kevent(kq, NULL, 0, event_list, BACKLOG, NULL);
        if (nev == -1)
        {
            perror("kevent");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nev; i++)
        {
            if (event_list[i].ident == server_fd)
            {
                // 处理新连接
                client_fd = accept(server_fd, NULL, NULL);
                if (client_fd == -1)
                {
                    perror("accept");
                    continue;
                }

                // 将新连接的可读事件加入kqueue
                EV_SET(&change_event,
                       client_fd,
                       EVFILT_READ,
                       EV_ADD | EV_ENABLE,
                       0,
                       0,
                       NULL);
                if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1)
                {
                    perror("kevent");
                    close(client_fd);
                    continue;
                }
            }
            else if (event_list[i].filter == EVFILT_READ)
            {
                // 处理客户端的可读事件
                handle_client(event_list[i].ident);
            }
        }
    }

    // 关闭监听套接字
    close(server_fd);
    return 0;
}
