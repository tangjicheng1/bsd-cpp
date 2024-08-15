#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_THREADS 2

bool is_lock = false; // 控制是否使用锁
std::mutex mtx;

void send_messages(int sock, const std::string& message)
{
    while (true)
    {
        if (is_lock)
        {
            std::lock_guard<std::mutex> lock(mtx);
            send(sock, message.c_str(), message.length(), 0);
        }
        else
        {
            send(sock, message.c_str(), message.length(), 0);
        }
        // std::this_thread::sleep_for(
        //     std::chrono::milliseconds(100)); // 控制发送速度
    }
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // 需要发送的单词列表
    std::vector<std::string> words = {
        "The night that Stephen Paddock opened fire on thousands of people at "
        "a Las Vegas country music concert...",
        "123456789",
        "fooooofooooofooooofooooofooooofooooo",
        "baaaaarbaaaaarbaaaaarbaaaaarbaaaaarbaaaaarbaaaaarbaaaaar",
        "bazbazbazbazbazbazbaz",
        "qqqqxqqqqxqqqqxqqqqxqqqqxqqqqx",
        "quux",
        "corge",
        "grault",
        "garply"
    };
    std::vector<std::thread> threads;

    // 创建线程并发送不同的单词
    for (int i = 0; i < MAX_THREADS; ++i)
    {
        threads.emplace_back(send_messages, sock, words[i]);
    }

    // 等待所有线程完成（实际上它们会一直运行）
    for (auto& t : threads)
    {
        t.join();
    }

    close(sock);
    return 0;
}
