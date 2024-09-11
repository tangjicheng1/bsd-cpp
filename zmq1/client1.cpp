#include "zmq.hpp"
#include <iostream>
#include <string>

int main() {
    // 创建一个 ZeroMQ 上下文
    zmq::context_t context(1);

    // 创建一个 REQ（请求）套接字，连接到服务器
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:5555");

    for (int i = 0; i < 5; ++i) {
        // 发送请求到服务器
        std::string request = "你好，服务器";
        zmq::message_t req_msg(request.size());
        memcpy(req_msg.data(), request.c_str(), request.size());
        std::cout << "发送的请求: " << request << std::endl;
        socket.send(req_msg, zmq::send_flags::none);

        // 接收服务器的响应
        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        std::string reply_str(static_cast<char*>(reply.data()), reply.size());
        std::cout << "接收到的响应: " << reply_str << std::endl;
    }

    return 0;
}
