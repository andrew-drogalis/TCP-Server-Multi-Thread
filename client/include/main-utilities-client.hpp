// Copyright 2024, Andrew Drogalis
// GNU License

#include <unistd.h>// for optarg, getopt

#include <iostream> // for operator<<, basic_ostream, cerr, cout
#include <print>    // for print
#include <regex>    // for regex, regex_match
#include <stdexcept>// for invalid_argument
#include <string>   // for stoi, basic_string

namespace tcpclient
{

bool validateMainParameters(int argc, char* argv[], std::string& IP_ADDRESS, int& PORT, int& BUFFER_SIZE)
{
    if (argc > 7)
    {
        std::cerr << "Too Many Arguments - Only (3) Arguments: -i [IP Address] -p [Port Number] -b [Buffer Size]\n";
        return false;
    }
    if (argc == 1)
    {
        return true;
    }

    std::regex ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

    int c;
    while ((c = getopt(argc, argv, "p:b:i:")) != -1)
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
        default:
            std::cerr << "Incorrect Argument. Flags are -i [IP Address] -p [Port Number] -b [Buffer Size]\n";
            return false;
        }
    }
    std::print("IP Address: {}, Port: {}, Buffer Size: {}", IP_ADDRESS, PORT, BUFFER_SIZE);
    // ------------------
    return true;
}

}// namespace tcpclient
