#include "zmq.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

int main() {
    zmq::context_t context(1);

    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    while (true) {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::string req_str(static_cast<char*>(request.data()), request.size());
        std::cout << "server recv: " << req_str << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string reply = "client, hello";
        zmq::message_t response(reply.size());
        memcpy(response.data(), reply.c_str(), reply.size());
        socket.send(response, zmq::send_flags::none);
    }

    return 0;
}
