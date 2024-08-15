#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket, max_sd, sd, activity, valread;
    int client_socket[30];
    int max_clients = 30;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    // Initialize client_socket array to 0
    for (int i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    // Create a master socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket options
    int opt = 1;
    if (setsockopt(
            server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    std::cout << "Listening on port " << PORT << std::endl;

    while (true)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server_fd to set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add child sockets to set
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        // Wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            std::cout << "select error" << std::endl;
        }

        // If something happened on the master socket, then it's an incoming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            if ((new_socket = accept(server_fd,
                                     (struct sockaddr*)&address,
                                     (socklen_t*)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            std::cout << "New connection, socket fd is " << new_socket
                      << ", ip is: " << inet_ntoa(address.sin_addr)
                      << ", port: " << ntohs(address.sin_port) << std::endl;

            // Add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    std::cout << "Adding to list of sockets at index " << i
                              << std::endl;
                    break;
                }
            }
        }

        // Else, it's some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0)
                {
                    // Somebody disconnected, get his details and print
                    getpeername(
                        sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    std::cout << "Host disconnected, ip "
                              << inet_ntoa(address.sin_addr) << ", port "
                              << ntohs(address.sin_port) << std::endl;

                    // Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    // Print the message from the client
                    buffer[valread] = '\0';
                    std::cout << "Message received: " << buffer << std::endl;
                }
            }
        }
    }

    return 0;
}
