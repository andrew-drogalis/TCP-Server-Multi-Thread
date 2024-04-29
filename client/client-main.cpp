// Copyright 2024, Andrew Drogalis
// GNU License

#include <iostream>// for operator<<, basic_ostream, cout

#include "main-utilities-client.hpp"// for validateMainParameters
#include "tcp-client.h"             // for TCPClient

int main(int argc, char* argv[])
{
    int port_number, buffer_size;
    struct hostent* server_ip;

    bool validate_response = tcpclient::validateMainParameters(argc, argv, port_number, buffer_size, server_ip);

    if (! validate_response)
    {
        return 1;
    }

    tcpclient::TCPClient tcpClient {port_number, buffer_size, server_ip};

    bool tcp_client_response = tcpClient.start_client();

    if (! tcp_client_response)
    {
        return 1;
    }

    std::cout << "Connect Closed Successfully.";

    return 0;
}
