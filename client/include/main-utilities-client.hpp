// Copyright 2024, Andrew Drogalis
// GNU License

#include <netdb.h> // for gethostbyname, herror, hostent (ptr only)
#include <stdio.h> // for NULL
#include <unistd.h>// for optarg, getopt

#include <iostream> // for operator<<, basic_ostream, cerr, cout
#include <print>    // for print
#include <stdexcept>// for invalid_argument
#include <string>   // for stoi, basic_string

namespace tcpclient
{

bool validateMainParameters(int argc, char* argv[], int& port_number, int buffer_size, hostent* server_ip)
{
    if (argc == 1)
    {
        return true;
    }
    if (argc > 7)
    {
        std::cerr << "Too Many Arguments - Only (3) Arguments: -i [IP Address], -p [Port Number] -b [Buffer Size]\n";
        return false;
    }
    int c;
    while ((c = getopt(argc, argv, "i:p:b:")) != -1)
    {
        switch (c)
        {

        case 'i':
            if ((server_ip = gethostbyname(optarg)) == NULL)
            {
                herror(" gethostbyname ");
                return false;
            }
            break;

        case 'p': {
            try
            {
                port_number = std::stoi(optarg);
            }
            catch (std::invalid_argument const& e)
            {
                std::cout << " ";
                return false;
            }
            break;
        }
        case 'b': {
            try
            {
                buffer_size = std::stoi(optarg);
            }
            catch (std::invalid_argument const& e)
            {
                std::cout << " ";
                return false;
            }
            break;
        }
        default:
            std::cerr << "Incorrect Argument. Flags are -i [IP Address], -p [Port Number] -b [Buffer Size]\n";
            return false;
        }
    }
    std::print("Port: {}, IP Address: {}, Buffer Size: {}", port_number, server_ip->h_name, buffer_size);

    return true;
}

}// namespace tcpclient
