#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds_socket"

int main() {
    int client_fd;
    struct sockaddr_un client_addr;
    char buffer[100];

    client_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, SOCKET_PATH, sizeof(client_addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr*)&client_addr, sizeof(struct sockaddr_un)) < 0) {
        perror("connect error");
        close(client_fd);
        exit(1);
    }

    const char* message = "Hello from UDS client!";
    write(client_fd, message, strlen(message));

    memset(buffer, 0, sizeof(buffer));
    int n = read(client_fd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("read error");
        close(client_fd);
        exit(1);
    }

    printf("Received message from server: %s\n", buffer);

    close(client_fd);

    return 0;
}
