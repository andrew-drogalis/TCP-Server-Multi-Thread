// Copyright 2024, Andrew Drogalis
// GNU License

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <expected>// for expected

#include "tcp-server-exception.h"// for TCPServerException

namespace tcpserver
{

class TCPServer
{

  public:
    TCPServer() = default;

    TCPServer(int port, int max_events, int max_clients, int buffer_size);

    [[nodiscard]] std::expected<bool, TCPServerException> start_server() noexcept;

    void clean_up_socket_epoll(int socket_FD, int epoll_FD) noexcept;

    static void handle_client(int clientFd, int buffer_size) noexcept;

  private:
    int port, max_events, max_clients, buffer_size;
};

}// namespace tcpserver

#endif
