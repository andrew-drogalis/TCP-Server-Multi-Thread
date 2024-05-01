// Copyright 2024, Andrew Drogalis
// GNU License

#include "tcp-client.h"

#include <arpa/inet.h> // for htons, inet_ntoa
#include <netdb.h>     // for hostent, h_addr
#include <netinet/in.h>// for sockaddr_in, in_addr
#include <sys/socket.h>// for AF_INET, connect, recv, send, socket, SOCK_S...
#include <unistd.h>    // for close

#include <cstring>        // for memcpy, memset
#include <expected>       // for expected
#include <iostream>       // for operator<<, basic_ostream, cout, basic_ostre...
#include <source_location>// for source_location
#include <string>         // for char_traits, basic_string, getline, operator<<

#include "tcp-client-exception.h"// for TCPClientException

namespace tcpclient
{

TCPClient::TCPClient(std::string ip_address, int port, int buffer_size) : ip_address(ip_address), port(port), buffer_size(buffer_size) {}

std::expected<bool, TCPClientException> TCPClient::start_client() noexcept
{
    int socket_FD = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_FD == -1)
    {
        return std::expected<bool, TCPClientException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to connect to Socket."};
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip_address.c_str(), &server.sin_addr);

    if (connect(socket_FD, (sockaddr*)&server, sizeof(server)) == -1)
    {
        clean_up_socket(socket_FD);
        return std::expected<bool, TCPClientException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to open connection to IP Address on specified port"};
    }
    // ------------------
    std::cout << "*********** TCP Client ***********\n\n";
    std::cout << "Established Connection to: " << inet_ntoa(server.sin_addr) << " on port: " << port << "\n\n";

    char buf[buffer_size];
    std::string user_input;
    while (true)
    {
        std::cout << "> ";
        getline(std::cin, user_input);

        if (user_input == "EXIT")
        {
            break;
        }

        int send_result = send(socket_FD, user_input.c_str(), user_input.size() + 1, 0);
        if (send_result == -1)
        {
            std::cout << "Failed to send message to server.\n";
            continue;
        }
        // ------------------
        memset(buf, 0, buffer_size);
        int bytes_received = recv(socket_FD, buf, buffer_size, 0);
        if (bytes_received == -1)
        {
            std::cout << "Failed to get response from server.\n";
        }
        else
        {
            std::cout << "Server Response> " << std::string(buf, bytes_received) << "\n";
        }
    }
    // ------------------
    clean_up_socket(socket_FD);
    return std::expected<bool, TCPClientException> {true};
}

void TCPClient::clean_up_socket(int socket_FD) noexcept
{
    if (socket_FD != -1)
    {
        close(socket_FD);
    }
}

}// namespace tcpclient
