// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <cstdint> // for uint16_t
#include <iostream>// for operator<<, basic_ostream, cout
#include <string>  // for basic_string

#include "client.h"                 // for Client
#include "main-utilities-client.hpp"// for validateClientParameters

int main(int argc, char* argv[])
{
    std::string IP_ADDRESS = "127.0.0.1";
    uint16_t PORT          = 54000;
    uint32_t BUFFER_SIZE   = 1024;
    char TCP_UDP           = 'U';
    bool BENCHMARK         = true;

    if (! dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK))
    {
        return 1;
    }
    dro::Client tcpClient {IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK};

    auto tcp_client_response = tcpClient.start_client();

    if (! tcp_client_response)
    {
        std::cout << "Error Location: " << tcp_client_response.error().where() << '\n';
        std::cout << tcp_client_response.error().what() << '\n';
        return 1;
    }

    std::cout << "\nConnection Closed Successfully.\n";
    return 0;
}
