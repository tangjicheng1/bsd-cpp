#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void readFile(const std::string& filename, std::vector<std::string>& messages) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        messages.push_back(line + "\n");  // 添加\n作为消息的结尾
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <server_ip:port>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string server_info = argv[2];
    std::string server_ip = server_info.substr(0, server_info.find(':'));
    int server_port = std::stoi(server_info.substr(server_info.find(':') + 1));

    std::vector<std::string> messages;
    readFile(filename, messages);
    messages.push_back(std::string("DONE"));

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (const auto& message : messages) {
        if (sendto(sock, message.c_str(), message.size(), 0, (sockaddr*)&server_addr,
                   sizeof(server_addr)) < 0) {
            perror("sendto");
            close(sock);
            return 1;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    double total_time = elapsed.count();
    std::cout << "Sent " << messages.size() << " messages in " << total_time << " seconds."
              << std::endl;
    std::cout << "Messages per second: " << messages.size() / total_time << std::endl;

    close(sock);
    return 0;
}
