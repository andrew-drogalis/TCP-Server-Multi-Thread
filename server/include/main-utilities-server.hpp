// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <unistd.h>// for optarg, getopt

#include <cstdint>  // for uint16_t
#include <iostream> // for operator<<, basic_ostream, cerr, cout
#include <print>    // for print
#include <stdexcept>// for invalid_argument
#include <string>   // for stoi, basic_string

namespace dro
{

void printServerUsage() { std::print("Usage: -p [Port Number] -c [Max Clients] -e [Max Events] -b [Buffer Size] -t [TCP/UDP]\n"); }

bool validateServerParameters(int argc, char* argv[], uint16_t& PORT, uint32_t& MAX_CLIENTS, uint32_t& MAX_EVENTS, uint32_t& BUFFER_SIZE,
                              char& TCP_UDP)
{
    if (argc > 11)
    {
        std::cerr << "Too Many Arguments - Only (5) Arguments\n";
        printServerUsage();
        return false;
    }
    if (argc == 1)
    {
        return true;
    }

    int c;
    while ((c = getopt(argc, argv, "p:c:e:b:t:")) != -1)
    {
        switch (c)
        {
        case 'p': {
            try
            {
                PORT = std::stoi(optarg);
                if (PORT < 0 || PORT > 65535)
                {
                    std::cerr << "Provide Valid Port Number 0 - 65535.";
                    return false;
                }
            }
            catch (std::invalid_argument const& e)
            {
                std::cerr << "Provide Integer for PORT. " << e.what();
                return false;
            }
            break;
        }
        case 'c': {
            try
            {
                MAX_CLIENTS = std::stoi(optarg);
                if (MAX_CLIENTS < 0)
                {
                    std::cerr << "Provide Positive value for Max Clients.";
                    return false;
                }
            }
            catch (std::invalid_argument const& e)
            {
                std::cerr << "Provide Integer for MAX_CLIENTS. " << e.what();
                return false;
            }
            break;
        }
        case 'e': {
            try
            {
                MAX_EVENTS = std::stoi(optarg);
                if (MAX_EVENTS < 0)
                {
                    std::cerr << "Provide Positive value for Max Events.";
                    return false;
                }
            }
            catch (std::invalid_argument const& e)
            {
                std::cerr << "Provide Integer for MAX_EVENTS. " << e.what();
                return false;
            }
            break;
        }
        case 'b': {
            try
            {
                BUFFER_SIZE = std::stoi(optarg);
                if (BUFFER_SIZE < 0)
                {
                    std::cerr << "Provide Positive value for Buffer Size.";
                    return false;
                }
            }
            catch (std::invalid_argument const& e)
            {
                std::cerr << "Provide Integer for BUFFER_SIZE. " << e.what();
                return false;
            }
            break;
        }
        case 't': {
            TCP_UDP = *optarg;
            if (TCP_UDP != 'T' && TCP_UDP != 'U')
            {
                std::cerr << "Provide either 'T' or 'U' for TCP/UDP.";
                return false;
            }
            break;
        }
        default:
            std::cerr << "Incorrect Argument.\n";
            printServerUsage();
            return false;
        }
    }
    return true;
}

}// namespace dro
