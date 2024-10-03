// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "client.h"

#include <arpa/inet.h>// for htons, inet_ntoa
#include <cstdint>
#include <netdb.h>     // for hostent, h_addr
#include <netinet/in.h>// for sockaddr_in, in_addr
#include <sys/socket.h>// for AF_INET, connect, recv, send, socket, SOCK_S...
#include <unistd.h>    // for close

#include <chrono>         // for high_resolution_clock
#include <cstdint>        // for uint16_t
#include <cstring>        // for memcpy, memset
#include <expected>       // for expected
#include <iostream>       // for operator<<, basic_ostream, cout, basic_ostre...
#include <source_location>// for source_location
#include <string>         // for char_traits, basic_string, getline, operator<<

#include "client-exception.h"// for ClientException

namespace dro
{

Client::Client(std::string ip_address, uint16_t port, uint32_t buffer_size, char tcp_udp, bool benchmark)
    : ip_address_(ip_address), port_(port), buffer_size_(buffer_size), tcp_udp_(tcp_udp), benchmark_(benchmark)
{
}

std::expected<bool, ClientException> Client::start_client() noexcept
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
        return std::expected<bool, ClientException> {std::unexpect, std::source_location::current().function_name(), "Failed to connect to Socket."};
    }

    server_.sin_family = AF_INET;
    server_.sin_port   = htons(port_);
    inet_pton(AF_INET, ip_address_.c_str(), &server_.sin_addr);

    if (connect(socket_FD, (sockaddr*)&server_, sizeof(server_)) == -1)
    {
        clean_up_socket(socket_FD);
        return std::expected<bool, ClientException> {std::unexpect, std::source_location::current().function_name(),
                                                     "Failed to open connection to IP Address on specified port"};
    }
    std::cout << "*********** Client ***********\n\n";
    std::cout << "Established Connection to: " << inet_ntoa(server_.sin_addr) << " on port: " << port_ << "\n\n";

    if (benchmark_)
    {
        run_benchmark(socket_FD);
    }
    else
    {
        handle_user_input(socket_FD);
    }
    clean_up_socket(socket_FD);
    return std::expected<bool, ClientException> {true};
}

void Client::handle_user_input(int socket_FD) noexcept
{
    char buf[buffer_size_];
    std::string user_input;
    while (true)
    {
        std::cout << "> ";
        getline(std::cin, user_input);

        if (user_input == "EXIT")
        {
            break;
        }
        if (tcp_udp_ == 'T' && send_tcp(socket_FD, user_input))
        {
            continue;
        }
        else if (tcp_udp_ == 'U' && send_udp(socket_FD, user_input))
        {
            continue;
        }

        memset(buf, 0, buffer_size_);
        int bytes_received;
        if (tcp_udp_ == 'T')
        {
            bytes_received = receive_tcp(socket_FD, buf);
        }
        else
        {
            bytes_received = receive_udp(socket_FD, buf);
        }
        if (bytes_received == -1)
        {
            std::cout << "Failed to get response from server.\n";
        }
        else
        {
            std::cout << "Server Response> " << std::string(buf, bytes_received) << "\n";
        }
    }
}

bool Client::send_tcp(int socket_FD, std::string& user_input) noexcept
{
    int send_result = send(socket_FD, user_input.c_str(), user_input.size() + 1, 0);
    if (send_result == -1)
    {
        std::cout << "Failed to send message to server.\n";
        return 1;
    }
    return 0;
}

bool Client::send_udp(int socket_FD, std::string& user_input) noexcept
{
    socklen_t serverlen = sizeof(server_);
    int send_result     = sendto(socket_FD, user_input.c_str(), user_input.size() + 1, 0, (sockaddr*)&server_, serverlen);
    if (send_result == -1)
    {
        std::cout << "Failed to send message to server.\n";
        return 1;
    }
    return 0;
}

int Client::receive_tcp(int socket_FD, char buf[]) noexcept { return recv(socket_FD, buf, buffer_size_, 0); }

int Client::receive_udp(int socket_FD, char buf[]) noexcept
{
    socklen_t serverlen = sizeof(server_);
    return recvfrom(socket_FD, buf, buffer_size_, 0, (sockaddr*)&server_, &serverlen);
}

void Client::run_benchmark(int socket_FD) noexcept
{
    std::cout << "Running Benchmark...\n";
    char buf[buffer_size_];
    memset(buf, 1, buffer_size_);
    long int const iters {100'000};

    auto start = std::chrono::high_resolution_clock::now();
    for (int i {}; i < iters; ++i)
    {
        int send_result = send(socket_FD, buf, buffer_size_, 0);
        if (send_result == -1)
        {
            std::cout << "Benchmark Error: failed to send message to server.\n";
            break;
        }
        int bytes_received = recv(socket_FD, buf, buffer_size_, 0);
        if (bytes_received == -1)
        {
            std::cout << "Benchmark Error: failed to get response from server.\n";
            break;
        }
    }
    auto stop            = std::chrono::high_resolution_clock::now();
    long int nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

    std::cout << "Average Time of: " << nanoseconds / iters << " ns per message. \n";
    std::cout << "Average Sent: " << iters * 1'000'000'000 / std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count()
              << " messages per second. \n";
}

void Client::clean_up_socket(int socket_FD) noexcept
{
    if (socket_FD != -1)
    {
        close(socket_FD);
    }
}

}// namespace dro
