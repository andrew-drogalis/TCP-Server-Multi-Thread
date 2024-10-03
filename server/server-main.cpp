// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <cstdint> // for uint16_t
#include <iostream>// for operator<<, basic_ostream, cout

#include "main-utilities-server.hpp"// for validateServerParameters
#include "server.h"                 // for Server

int main(int argc, char* argv[])
{
    // User Default Values
    uint16_t PORT        = 54000;
    uint32_t MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'U';

    if (! dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP))
    {
        return 1;
    }

    dro::Server tcpServ {PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP};
    auto tcp_server_response = tcpServ.start_server();
    if (! tcp_server_response)
    {
        std::cout << "Error Location: " << tcp_server_response.error().where() << '\n';
        std::cout << tcp_server_response.error().what() << '\n';
        return 1;
    }

    std::cout << "\nConnection Closed Successfully.\n";
    return 0;
}
