// Copyright 2024, Andrew Drogalis
// GNU License

#include "tcp-client.h"

#include <arpa/inet.h> // for htons, inet_ntoa
#include <netdb.h>     // for hostent, h_addr
#include <netinet/in.h>// for sockaddr_in, in_addr
#include <sys/socket.h>// for AF_INET, connect, recv, send, socket, SOCK_S...
#include <unistd.h>    // for close

#include <cstring> // for memcpy, memset
#include <iostream>// for operator<<, basic_ostream, cout, basic_ostre...
#include <string>  // for char_traits, basic_string, getline, operator<<

#include "tcp-client-exception.h"// for TCPClientException

namespace tcpclient
{

TCPClient::TCPClient(int port, int buffer_size, struct hostent* server_ip) {}

bool TCPClient::start_client()
{
    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD == -1)
    {
        std::cerr << "Failed to connect to Socket.";
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy(&server.sin_addr, server_ip->h_addr, server_ip->h_length);

    int connectRes = connect(sockFD, (sockaddr*)&server, sizeof(server));
    if (connectRes == -1)
    {
        return false;
    }

    std::cout << "Established Connection to: " << inet_ntoa(server.sin_addr) << " on port: " << port << "\n\n";

    // While loop:
    char buf[buffer_size];
    std::string userInput;

    while (true)
    {
        // Enter lines of text
        std::cout << "> ";
        getline(std::cin, userInput);

        // Send to server
        int sendRes = send(sockFD, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1)
        {
            std::cout << "Failed to send to server!\n";
            continue;
        }

        // Wait for response
        memset(buf, 0, buffer_size);
        int bytesReceived = recv(sockFD, buf, buffer_size, 0);
        if (bytesReceived == -1)
        {
            std::cout << "There was an error getting response from server\n";
        }
        else
        {
            std::cout << "SERVER> " << std::string(buf, bytesReceived) << "\n";
        }
    }

    close(sockFD);
    return true;
}

}// namespace tcpclient
