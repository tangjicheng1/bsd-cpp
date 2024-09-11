#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }

    std::vector<std::string> messages;
    char buffer[BUFFER_SIZE];
    pollfd fds[1];
    fds[0].fd = sock;
    fds[0].events = POLLIN;

    auto start_time = std::chrono::high_resolution_clock::time_point();
    auto end_time = std::chrono::high_resolution_clock::time_point();
    bool first_message = true;
    int message_count = 0;

    while (true) {
        int poll_count = poll(fds, 1, -1);
        if (poll_count < 0) {
            perror("poll");
            close(sock);
            return 1;
        }

        if (fds[0].revents & POLLIN) {
            sockaddr_in client_addr{};
            socklen_t client_addr_len = sizeof(client_addr);
            ssize_t received =
                recvfrom(sock, buffer, BUFFER_SIZE, 0, (sockaddr*)&client_addr, &client_addr_len);
            if (received < 0) {
                perror("recvfrom");
                close(sock);
                return 1;
            }

            std::string message(buffer, received);
            messages.push_back(message);
            message_count++;

            if (first_message) {
                start_time = std::chrono::high_resolution_clock::now();
                first_message = false;
            }

            if (message == std::string("DONE")) {
                end_time = std::chrono::high_resolution_clock::now();
                break;
            }
        }
    }

    std::chrono::duration<double> elapsed = end_time - start_time;
    double total_time = elapsed.count();

    std::cout << "Received " << message_count << " messages in " << total_time << " seconds."
              << std::endl;
    std::cout << "Messages per second: " << message_count / total_time << std::endl;

    close(sock);
    return 0;
}
