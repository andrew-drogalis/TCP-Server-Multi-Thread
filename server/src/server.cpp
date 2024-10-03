// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "server.h"

#include <arpa/inet.h> // for htonl, htons
#include <netinet/in.h>// for sockaddr_in, INADDR_ANY, in_addr
#include <sys/epoll.h> // for epoll_event, epoll_ctl, EPOLLIN, EPOLL_CTL_ADD
#include <sys/socket.h>// for AF_INET, accept, bind, listen, socket, SOCK_...
#include <unistd.h>    // for close, read, write

#include <chrono>// for high_resolution_clock
#include <cstdint>
#include <expected>       // for expected
#include <iostream>       // for operator<<, basic_ostream, cout
#include <source_location>// for source_location
#include <thread>         // for thread

#include "server-exception.h"// for ServerException
#include "thread-pool.h"     // for ThreadPool

namespace dro
{

Server::Server(uint16_t port, uint32_t max_events, uint32_t max_clients, uint32_t buffer_size, char tcp_udp)
    : port_(port), max_events_(max_events), max_clients_(max_clients), buffer_size_(buffer_size), tcp_udp_(tcp_udp)
{
}

std::expected<bool, ServerException> Server::start_server() noexcept
{
    int socket_FD;
    if (tcp_udp_ == 'T')
    {
        socket_FD = socket(AF_INET, SOCK_STREAM, 0);
    }
    else
    {
        socket_FD = socket(AF_INET, SOCK_DGRAM, 0);
    }

    if (socket_FD == -1)
    {
        return std::expected<bool, ServerException> {std::unexpect, std::source_location::current().function_name(), "Failed to connect to Socket."};
    }

    server_.sin_family      = AF_INET;
    server_.sin_addr.s_addr = htonl(INADDR_ANY);
    server_.sin_port        = htons(port_);

    if (bind(socket_FD, (sockaddr*)&server_, sizeof(server_)) == -1)
    {
        clean_up_socket_epoll(socket_FD, -1);
        return std::expected<bool, ServerException> {std::unexpect, std::source_location::current().function_name(),
                                                     "Failed to open connection to IP Address on specified port"};
    }
    std::cout << "*********** Server ***********\n\n";
    std::cout << "Server is Online.\n";

    if (tcp_udp_ == 'T')
    {
        return run_tcp_server(socket_FD);
    }
    else
    {
        return run_udp_server(socket_FD);
    }
}

std::expected<bool, ServerException> Server::run_tcp_server(int socket_FD) noexcept
{

    if (listen(socket_FD, max_clients_) == -1)
    {
        clean_up_socket_epoll(socket_FD, -1);
        return std::expected<bool, ServerException> {std::unexpect, std::source_location::current().function_name(), "Listening Failed"};
    }

    int epoll_FD = epoll_create1(0);
    if (epoll_FD == -1)
    {
        clean_up_socket_epoll(socket_FD, epoll_FD);
        return std::expected<bool, ServerException> {std::unexpect, std::source_location::current().function_name(),
                                                     "Failed to create epoll instance."};
    }

    struct epoll_event event, events[max_events_];
    event.events  = EPOLLIN;
    event.data.fd = socket_FD;
    if (epoll_ctl(epoll_FD, EPOLL_CTL_ADD, socket_FD, &event) == -1)
    {
        clean_up_socket_epoll(socket_FD, epoll_FD);
        return std::expected<bool, ServerException> {std::unexpect, std::source_location::current().function_name(),
                                                     "Failed to add server socket to epoll instance."};
    }
    std::cout << "Listening for connections on Port: " << port_ << '\n';
    threadPool_.start();

    while (true)
    {
        int num_events = epoll_wait(epoll_FD, events, max_events_, -1);
        if (num_events == -1)
        {
            clean_up_socket_epoll(socket_FD, epoll_FD);
            return std::expected<bool, ServerException> {std::unexpect, std::source_location::current().function_name(),
                                                         "Failed wait for epoll events."};
        }
        for (int i = 0; i < num_events; ++i)
        {
            if (events[i].data.fd == socket_FD)
            {
                struct sockaddr_in client_address;
                socklen_t client_address_length = sizeof(client_address);
                int client_FD                   = accept(socket_FD, (struct sockaddr*)&client_address, &client_address_length);
                if (client_FD == -1)
                {
                    std::cerr << "Failed to accept client connection.\n";
                    continue;
                }

                event.events  = EPOLLIN;
                event.data.fd = client_FD;
                if (epoll_ctl(epoll_FD, EPOLL_CTL_ADD, client_FD, &event) == -1)
                {
                    std::cerr << "Failed to add client socket to epoll instance.\n";
                    close(client_FD);
                    continue;
                }

                std::thread client_thread(handle_client, client_FD, buffer_size_);
                client_thread.detach();
            }
            else
            {
                int client_FD         = events[i].data.fd;
                uint32_t local_buffer = buffer_size_;
                threadPool_.queue_job([&client_FD, &local_buffer] { dro::Server::handle_client(client_FD, local_buffer); });
            }
        }
    }
    threadPool_.stop();
    clean_up_socket_epoll(socket_FD, epoll_FD);
    return std::expected<bool, ServerException> {true};
}

void Server::handle_client(int client_FD, int buffer_size) noexcept
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
    close(client_FD);
}

std::expected<bool, ServerException> Server::run_udp_server(int socket_FD) noexcept
{
    socklen_t serverlen = sizeof(server_);
    char buffer[buffer_size_];
    while (true)
    {
        int bytes_received = recvfrom(socket_FD, buffer, buffer_size_, 0, (struct sockaddr*)&server_, &serverlen);
        if (bytes_received < 0)
        {
            std::cout << "Failed to receive UDP message\n";
            continue;
        }
        int send_result = sendto(socket_FD, buffer, buffer_size_, 0, (sockaddr*)&server_, serverlen);
        if (send_result == -1)
        {
            std::cout << "Failed to send UDP message\n";
            continue;
        }
    }
    clean_up_socket_epoll(socket_FD, -1);
    return std::expected<bool, ServerException> {true};
}

void Server::clean_up_socket_epoll(int socket_FD, int epoll_FD) noexcept
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
}// namespace dro
