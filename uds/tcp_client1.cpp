// tcp_client.cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./tcp_client <server_ip> <message_file>\n";
        return EXIT_FAILURE;
    }

    std::string server_ip = argv[1];
    std::string filename = argv[2];
    std::vector<std::string> messages;

    // 从文件中读取消息
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(infile, line)) {
        messages.push_back(line + "\n");  // 添加 '\n' 到每条消息的末尾
    }
    infile.close();

    std::cout << "Total messages to send: " << messages.size() << "\n";

    // 创建套接字
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error\n";
        return EXIT_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 转换 IPv4 地址
    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported\n";
        return EXIT_FAILURE;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server " << server_ip << ":" << PORT << "\n";

    // 开始计时
    auto start_time = std::chrono::high_resolution_clock::now();

    // 发送消息
    for (const auto& msg : messages) {
        send(sock, msg.c_str(), msg.size(), 0);
    }

    // 结束计时
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // 计算每秒发送的消息数量
    double total_time = elapsed.count();
    double messages_per_second = messages.size() / total_time;

    std::cout << "Total time: " << total_time << " seconds\n";
    std::cout << "Messages per second: " << messages_per_second << "\n";

    close(sock);
    return 0;
}
