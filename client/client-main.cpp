// Copyright 2024, Andrew Drogalis
// GNU License

#include <iostream>// for operator<<, basic_ostream, cout

#include "main-utilities-client.hpp"// for validateMainParameters
#include "tcp-client.h"             // for TCPClient

int main(int argc, char* argv[])
{
    int PORT = 54000, BUFFER_SIZE = 1024;
    struct hostent* SERVER_IP;

    if (! tcpclient::validateMainParameters(argc, argv, SERVER_IP, PORT, BUFFER_SIZE))
    {
        return 1;
    }
    // -------------------
    tcpclient::TCPClient tcpClient {SERVER_IP, PORT, BUFFER_SIZE};

    auto tcp_client_response = tcpClient.start_client();

    if (! tcp_client_response)
    {
        std::cout << "Error Location: " << tcp_client_response.error().where() << '\n';
        std::cout << tcp_client_response.error().what() << '\n';
        return 1;
    }

    std::cout << "Connect Closed Successfully.";
    // -------------------
    return 0;
}
