#include <zmq.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

int main() {
    // 创建一个 ZeroMQ 上下文
    zmq::context_t context(1);

    // 创建一个 REP（响应）套接字，绑定到指定端口
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    while (true) {
        // 接收客户端的请求
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::string req_str(static_cast<char*>(request.data()), request.size());
        std::cout << "接收到的请求: " << req_str << std::endl;

        // 模拟处理请求
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 发送响应给客户端
        std::string reply = "世界，你好";
        zmq::message_t response(reply.size());
        memcpy(response.data(), reply.c_str(), reply.size());
        socket.send(response, zmq::send_flags::none);
    }

    return 0;
}
