// Copyright 2024, Andrew Drogalis
// GNU License

#include <iostream>// for operator<<, basic_ostream, cout

#include "main-utilities-server.hpp"// for validateMainParameters
#include "tcp-server.h"             // for TCPServer

int main(int argc, char* argv[])
{
    int PORT, max_clients, max_events;

    bool validate_response = tcpserver::validateMainParameters(argc, argv, PORT, max_clients, max_events);

    if (! validate_response)
    {
        return 1;
    }

    tcpserver::TCPServer tcpServ {PORT, max_clients, max_events};

    bool server_response = tcpServ.initialize_server();

    if (! server_response)
    {
        return 1;
    }

    std::cout << "Connect Closed Successfully.";

    return 0;
}