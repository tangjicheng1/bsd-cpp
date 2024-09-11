#include <iostream>
#include <string>
#include "zmq.hpp"

int main() {
    zmq::context_t context(1);

    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:5555");

    for (int i = 0; i < 5; ++i) {
        std::string request = "hello, server.";
        zmq::message_t req_msg(request.size());
        memcpy(req_msg.data(), request.c_str(), request.size());
        std::cout << "send: " << request << std::endl;
        socket.send(req_msg, zmq::send_flags::none);

        zmq::message_t reply;
        auto received = socket.recv(reply, zmq::recv_flags::none);
        if (received == std::nullopt) {
            continue;
        }
        std::string reply_str(static_cast<char*>(reply.data()), reply.size());
        std::cout << "client recv: " << reply_str << std::endl;
    }

    return 0;
}
