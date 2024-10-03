// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_CLIENT_H
#define DRO_CLIENT_H

#include <expected>// for expected
#include <string>  // for basic_string
#include <cstdint> // for uint16_t

#include <netinet/in.h>// for sockaddr_in

#include "client-exception.h"// for ClientException

namespace dro
{

class Client
{

  public:
    Client() = default;

    Client(std::string ip_address, uint16_t port, uint32_t buffer_size, char tcp_udp, bool benchmark);

    [[nodiscard]] std::expected<bool, ClientException> start_client() noexcept;

    void clean_up_socket(int socket_FD) noexcept;

    void handle_user_input(int socket_FD) noexcept;

    bool send_tcp(int socket_FD, std::string& user_input) noexcept;

    bool send_udp(int socket_FD, std::string& user_input) noexcept;

    int receive_tcp(int socket_FD, char buf[]) noexcept;

    int receive_udp(int socket_FD, char buf[]) noexcept;

    void run_benchmark(int socket_FD) noexcept;

  private:
    std::string ip_address_;
    int port_, buffer_size_;
    char tcp_udp_;
    bool benchmark_;
    struct sockaddr_in server_;
};

}// namespace dro
#endif
