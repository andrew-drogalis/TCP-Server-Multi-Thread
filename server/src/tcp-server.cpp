// Copyright 2024, Andrew Drogalis
// GNU License

#include "tcp-server.h"

#include <arpa/inet.h> // for htonl, htons
#include <netinet/in.h>// for sockaddr_in, INADDR_ANY, in_addr
#include <sys/epoll.h> // for epoll_event, epoll_ctl, EPOLLIN, EPOLL_CTL_ADD
#include <sys/socket.h>// for AF_INET, accept, bind, listen, socket, SOCK_...
#include <unistd.h>    // for close, read, write

#include <expected>       // for expected
#include <iostream>       // for operator<<, basic_ostream, cout
#include <source_location>// for source_location
#include <thread>         // for thread

#include "tcp-server-exception.h"// for TCPServerException

namespace tcpserver
{

TCPServer::TCPServer(int port, int max_events, int max_clients, int buffer_size)
    : port(port), max_events(max_events), max_clients(max_clients), buffer_size(buffer_size)
{
}

std::expected<bool, TCPServerException> TCPServer::start_server()
{
    // AF_INET: internet socket, Protocol: 0, oti einai TCP
    // socket_file_descriptor
    int socket_FD = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_FD == -1)
    {
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(), "Can't create socket"};
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind(socket_FD, (sockaddr*)&server, sizeof(server)) == -1)
    {
        clean_up_socket_epoll(socket_FD, -1);
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(), "Can't bind to IP/portd"};
    }

    std::cout << "Server was successfully initialised...\n";

    if (listen(socket_FD, max_clients) == -1)
    {
        clean_up_socket_epoll(socket_FD, -1);
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(), "Listening Failed"};
    }

    int epoll_FD = epoll_create1(0);
    if (epoll_FD == -1)
    {
        clean_up_socket_epoll(socket_FD, epoll_FD);
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to create epoll instance."};
    }

    struct epoll_event event, events[max_events];
    // Add server socket to epoll
    event.events = EPOLLIN;
    event.data.fd = socket_FD;
    if (epoll_ctl(epoll_FD, EPOLL_CTL_ADD, socket_FD, &event) == -1)
    {
        clean_up_socket_epoll(socket_FD, epoll_FD);
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to add server socket to epoll instance."};
    }

    std::cout << "Listening for connections on port " << port << "\n";

    while (true)
    {
        int numEvents = epoll_wait(epoll_FD, events, max_events, -1);
        if (numEvents == -1)
        {
            std::cerr << "Failed to wait for events." << std::endl;
            break;
        }

        for (int i = 0; i < numEvents; ++i)
        {
            if (events[i].data.fd == socket_FD)
            {
                // Accept new client connection
                struct sockaddr_in clientAddress;
                socklen_t clientAddressLength = sizeof(clientAddress);
                int client_FD = accept(socket_FD, (struct sockaddr*)&clientAddress, &clientAddressLength);
                if (client_FD == -1)
                {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    continue;
                }

                // Add client socket to epoll
                event.events = EPOLLIN;
                event.data.fd = client_FD;
                if (epoll_ctl(epoll_FD, EPOLL_CTL_ADD, client_FD, &event) == -1)
                {
                    std::cerr << "Failed to add client socket to epoll instance." << std::endl;
                    close(client_FD);
                    continue;
                }

                // Create a new thread to handle the client connection
                std::thread client_thread(handle_client, client_FD, buffer_size);
                client_thread.detach();
            }
            else
            {
                // Handle client data
                int client_FD = events[i].data.fd;
                std::thread client_thread(handle_client, client_FD, buffer_size);
                client_thread.detach();
            }
        }
    }
    // -------------------
    clean_up_socket_epoll(socket_FD, epoll_FD);

    return std::expected<bool, TCPServerException> {true};
}

void TCPServer::clean_up_socket_epoll(int socket_FD, int epoll_FD)
{
    if (socket_FD != -1)
    {
        close(socket_FD);
    }
    if (epoll_FD != -1)
    {
        close(epoll_FD);
    }
}

void TCPServer::handle_client(int client_FD, int buffer_size)
{
    char buffer[buffer_size];
    while (true)
    {
        int bytes_read = read(client_FD, buffer, sizeof(buffer));
        if (bytes_read <= 0)
        {
            break;
        }
        write(client_FD, buffer, bytes_read);
    }
    // -------------------
    close(client_FD);
}

}// namespace tcpserver
