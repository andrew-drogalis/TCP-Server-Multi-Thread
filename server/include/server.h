// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_SERVER_H
#define DRO_SERVER_H

#include <cstdint>
#include <expected>// for expected

#include <netinet/in.h>// for sockaddr_in

#include "server-exception.h"// for ServerException
#include "thread-pool.h"     // for ThreadPool

namespace dro
{

class Server
{
  public:
    Server() = default;

    Server(uint16_t port, uint32_t max_events, uint32_t max_clients, uint32_t buffer_size, char tcp_udp);

    [[nodiscard]] std::expected<bool, ServerException> start_server() noexcept;

    [[nodiscard]] std::expected<bool, ServerException> run_tcp_server(int socket_FD) noexcept;

    static void handle_client(int clientFd, int buffer_size) noexcept;

    [[nodiscard]] std::expected<bool, ServerException> run_udp_server(int socket_FD) noexcept;

    void clean_up_socket_epoll(int socket_FD, int epoll_FD) noexcept;

  private:
    int port_, max_events_, max_clients_, buffer_size_;
    char tcp_udp_;
    struct sockaddr_in server_;
    ThreadPool threadPool_;
};

}// namespace dro
#endif
