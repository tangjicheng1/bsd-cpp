#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    char buffer[100];

    server_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_un)) < 0) {
        perror("bind error");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen error");
        close(server_fd);
        exit(1);
    }

    printf("Server is waiting for client connection...\n");

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept error");
        close(server_fd);
        exit(1);
    }

    memset(buffer, 0, sizeof(buffer));
    int n = read(client_fd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("read error");
        close(client_fd);
        close(server_fd);
        exit(1);
    }

    printf("Received message from client: %s\n", buffer);

    const char* response = "Hello from UDS server!";
    write(client_fd, response, strlen(response));

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
