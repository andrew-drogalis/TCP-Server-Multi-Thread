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

bool validateMainParameters(int argc, char* argv[], hostent* SERVER_IP, int& PORT, int BUFFER_SIZE)
{
    if (argc > 7)
    {
        std::cerr << "Too Many Arguments - Only (3) Arguments: -i [IP Address], -p [Port Number] -b [Buffer Size]\n";
        return false;
    }
    if (argc == 1)
    {
        return true;
    }

    int c;
    while ((c = getopt(argc, argv, "i:p:b:")) != -1)
    {
        switch (c)
        {
        case 'i':
            if ((SERVER_IP = gethostbyname(optarg)) == NULL)
            {
                herror(" gethostbyname ");
                return false;
            }
            break;

        case 'p': {
            try
            {
                PORT = std::stoi(optarg);
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
            }
            catch (std::invalid_argument const& e)
            {
                std::cerr << "Provide Integer for BUFFER_SIZE. " << e.what();
                return false;
            }
            break;
        }
        default:
            std::cerr << "Incorrect Argument. Flags are -i [IP Address], -p [Port Number] -b [Buffer Size]\n";
            return false;
        }
    }
    std::print("IP Address: {}, Port: {}, Buffer Size: {}", SERVER_IP->h_name, PORT, BUFFER_SIZE);
    // ------------------
    return true;
}

}// namespace tcpclient
