// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <unistd.h>// for optarg, getopt

#include <iostream> // for operator<<, basic_ostream, cerr, cout
#include <print>    // for print
#include <regex>    // for regex, regex_match
#include <stdexcept>// for invalid_argument
#include <string>   // for stoi, basic_string

namespace dro
{

void printClientUsage() { std::print("Usage: -i [IP Address] -p [Port Number] -b [Buffer Size] -t [TCP/UDP] -m [Benchmark]\n"); }

bool validateClientParameters(int argc, char* argv[], std::string& IP_ADDRESS, int& PORT, int& BUFFER_SIZE, char& TCP_UDP, bool& BENCHMARK)
{
    if (argc > 11)
    {
        std::cerr << "Too Many Arguments - Only (5) Arguments\n";
        printClientUsage();
        return false;
    }
    if (argc == 1)
    {
        return true;
    }

    std::regex ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    int c;
    while ((c = getopt(argc, argv, "p:b:i:t:m:")) != -1)
    {
        switch (c)
        {
        case 'i': {
            IP_ADDRESS = optarg;
            if (! regex_match(IP_ADDRESS, ipv4))
            {
                std::cerr << "Provide valid IPv4 address. e.g. 127.0.0.1";
                return false;
            }
            break;
        }
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
        case 'm': {
            BENCHMARK = (*optarg == '0') ? false : true;
            break;
        }
        default:
            std::cerr << "Incorrect Argument.\n";
            printClientUsage();
            return false;
        }
    }
    return true;
}

}// namespace dro
