// Copyright 2024, Andrew Drogalis
// GNU License

#include <iostream>// for operator<<, basic_ostream, cout

#include "main-utilities-server.hpp"// for validateMainParameters
#include "tcp-server.h"             // for TCPServer

int main(int argc, char* argv[])
{
    // User Default Values
    int PORT = 54000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    if (! tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE))
    {
        return 1;
    }
    // -------------------
    tcpserver::TCPServer tcpServ {PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE};

    auto server_response = tcpServ.start_server();

    if (! server_response)
    {
        std::cout << "Error Location: " << tcp_server_response.error().where() << '\n';
        std::cout << tcp_server_response.error().what() << '\n';
        return 1;
    }

    std::cout << "Connection Closed Successfully.\n";
    // -------------------
    return 0;
}