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

std::expected<bool, TCPServerException> TCPServer::start_server() noexcept
{
    int socket_FD = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_FD == -1)
    {
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to connect to Socket."};
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if (bind(socket_FD, (sockaddr*)&server, sizeof(server)) == -1)
    {
        clean_up_socket_epoll(socket_FD, -1);
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to open connection to IP Address on specified port"};
    }
    // ------------------
    std::cout << "*********** TCP Server ***********\n\n";
    std::cout << "Server is Online.\n";

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
    event.events = EPOLLIN;
    event.data.fd = socket_FD;
    if (epoll_ctl(epoll_FD, EPOLL_CTL_ADD, socket_FD, &event) == -1)
    {
        clean_up_socket_epoll(socket_FD, epoll_FD);
        return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(),
                                                        "Failed to add server socket to epoll instance."};
    }
    // ------------------
    std::cout << "Listening for connections on Port: " << port << '\n';

    while (true)
    {
        int num_events = epoll_wait(epoll_FD, events, max_events, -1);
        if (num_events == -1)
        {
            clean_up_socket_epoll(socket_FD, epoll_FD);
            return std::expected<bool, TCPServerException> {std::unexpect, std::source_location::current().function_name(),
                                                            "Failed wait for epoll events."};
        }

        for (int i = 0; i < num_events; ++i)
        {
            if (events[i].data.fd == socket_FD)
            {
                struct sockaddr_in client_address;
                socklen_t client_address_length = sizeof(client_address);
                int client_FD = accept(socket_FD, (struct sockaddr*)&client_address, &client_address_length);
                if (client_FD == -1)
                {
                    std::cerr << "Failed to accept client connection.\n";
                    continue;
                }

                event.events = EPOLLIN;
                event.data.fd = client_FD;
                if (epoll_ctl(epoll_FD, EPOLL_CTL_ADD, client_FD, &event) == -1)
                {
                    std::cerr << "Failed to add client socket to epoll instance.\n";
                    close(client_FD);
                    continue;
                }

                std::thread client_thread(handle_client, client_FD, buffer_size);
                client_thread.detach();
            }
            else
            {
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

void TCPServer::clean_up_socket_epoll(int socket_FD, int epoll_FD) noexcept
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

void TCPServer::handle_client(int client_FD, int buffer_size) noexcept
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
