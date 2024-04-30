// Copyright 2024, Andrew Drogalis
// GNU License

#include <unistd.h>// for optarg, getopt

#include <iostream> // for operator<<, basic_ostream, cerr, cout
#include <print>    // for print
#include <stdexcept>// for invalid_argument
#include <string>   // for stoi, basic_string

namespace tcpserver
{

bool validateMainParameters(int argc, char* argv[], int& PORT, int& MAX_CLIENTS, int& MAX_EVENTS, int& BUFFER_SIZE)
{
    if (argc > 9)
    {
        std::cerr << "Too Many Arguments - Only (4) Arguments: -p [Port Number] -c [Max Clients] -e [Max Events] -b [Buffer Size]\n";
        return false;
    }
    if (argc == 1)
    {
        return true;
    }

    int c;
    while ((c = getopt(argc, argv, "p:c:e:b:")) != -1)
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
        default:
            std::cerr << "Incorrect Argument. Flags are -p [Port Number] -c [Max Clients] -e [Max Events] -b [Buffer Size]\n";
            return false;
        }
    }
    std::print("Port: {}, Max Clients: {}, Max Events: {}, Buffer Size: {}", PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE);
    // ------------------
    return true;
}

}// namespace tcpserver
