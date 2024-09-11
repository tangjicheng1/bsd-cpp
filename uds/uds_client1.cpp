// uds_client.cpp
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define SOCKET_PATH "/tmp/uds_socket"
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./uds_client <message_file>\n";
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];
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

    // 创建 UDS 套接字
    int sock = 0;
    struct sockaddr_un serv_addr;
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error\n";
        return EXIT_FAILURE;
    }

    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

    // 连接到服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server " << SOCKET_PATH << "\n";

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
