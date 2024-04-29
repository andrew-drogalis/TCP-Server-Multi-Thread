// Copyright 2024, Andrew Drogalis
// GNU License

#include <unistd.h>// for optarg, getopt

#include <iostream> // for operator<<, basic_ostream, cerr, cout
#include <print>    // for print
#include <stdexcept>// for invalid_argument
#include <string>   // for stoi, basic_string

namespace tcpserver
{

bool validateMainParameters(int argc, char* argv[], int& PORT, int& max_clients, int& max_events)
{
    if (argc == 1)
    {
        return true;
    }
    if (argc > 9)
    {
        std::cerr << "Too Many Arguments - Only (3) Arguments: -p [Port Number] -c [Max Clients] -e [Max Events]\n";
        return false;
    }
    int c;
    while ((c = getopt(argc, argv, "p:c:e:")) != -1)
    {
        switch (c)
        {
        case 'p': {
            try
            {
                PORT = std::stoi(optarg);
            }
            catch (std::invalid_argument const& e)
            {
                std::cout << " ";
                return false;
            }
            break;
        }
        case 'c': {
            try
            {
                max_clients = std::stoi(optarg);
            }
            catch (std::invalid_argument const& e)
            {
                std::cout << " ";
                return false;
            }
            break;
        }
        case 'e': {
            try
            {
                max_events = std::stoi(optarg);
            }
            catch (std::invalid_argument const& e)
            {
                std::cout << " ";
                return false;
            }
            break;
        }
        default:
            std::cerr << "Incorrect Argument. Flags are -p [Port Number] -c [Max Clients] -e [Max Events]\n";
            return false;
        }
    }
    std::print("Port: {}, Max Clients: {}, MAx Events: {}", PORT, max_clients, max_events);

    return true;
}

}// namespace tcpserver
