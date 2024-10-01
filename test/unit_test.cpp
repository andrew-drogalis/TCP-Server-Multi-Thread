// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <string>

#include "gtest/gtest.h"

#include "client.h"
#include "main-utilities-client.hpp"
#include "main-utilities-server.hpp"
#include "server.h"

namespace
{

TEST(ServerTEST, Constructor)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';
    dro::Server tcpServ {PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP};

    SUCCEED();
}

TEST(ClientTEST, Constructor)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;
    dro::Client tcpClient {IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK};

    SUCCEED();
}

// SERVER UTILITIES
// ========================================================================================

TEST(ServerTEST, MainUtilitiesServer_Correct_1_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    int argc            = 1;
    char program_name[] = "TEST";
    char* argv[]        = {program_name};

    EXPECT_TRUE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

TEST(ServerTEST, MainUtilitiesServer_Correct_2_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    int argc            = 3;
    char program_name[] = "TEST";
    char port_flag[]    = "-p";
    char port[]         = "40000";
    char* argv[]        = {program_name, port_flag, port};

    EXPECT_TRUE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
    EXPECT_EQ(PORT, 40000);
}

TEST(ServerTEST, MainUtilitiesServer_Correct_3_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    int argc            = 3;
    char program_name[] = "TEST";
    char client_flag[]  = "-c";
    char max_clients[]  = "100";
    char* argv[]        = {program_name, client_flag, max_clients};

    EXPECT_TRUE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
    EXPECT_EQ(MAX_CLIENTS, 100);
}

TEST(ServerTEST, MainUtilitiesServer_Correct_4_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    int argc            = 3;
    char program_name[] = "TEST";
    char events_flag[]  = "-e";
    char max_events[]   = "101";
    char* argv[]        = {program_name, events_flag, max_events};

    EXPECT_TRUE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
    EXPECT_EQ(MAX_EVENTS, 101);
}

TEST(ServerTEST, MainUtilitiesServer_Correct_5_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    int argc            = 3;
    char program_name[] = "TEST";
    char buffer_flag[]  = "-b";
    char buffer_size[]  = "1000";
    char* argv[]        = {program_name, buffer_flag, buffer_size};

    EXPECT_TRUE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

TEST(ServerTEST, MainUtilitiesServer_Correct_6_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    int argc            = 9;
    char program_name[] = "TEST";
    char port_flag[]    = "-p";
    char port[]         = "40000";
    char client_flag[]  = "-c";
    char max_clients[]  = "100";
    char events_flag[]  = "-e";
    char max_events[]   = "101";
    char buffer_flag[]  = "-b";
    char buffer_size[]  = "1000";
    char* argv[]        = {program_name, buffer_flag, buffer_size, events_flag, max_events, client_flag, max_clients, port_flag, port};

    EXPECT_TRUE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
    EXPECT_EQ(PORT, 40000);
    EXPECT_EQ(MAX_CLIENTS, 100);
    EXPECT_EQ(MAX_EVENTS, 101);
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

// ========================================================================================

TEST(ServerTEST, MainUtilitiesServer_Negative_1_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    // Argc Too Large
    int argc            = 12;
    char program_name[] = "TEST";
    char* argv[]        = {program_name};

    EXPECT_FALSE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

TEST(ServerTEST, MainUtilitiesServer_Negative_2_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    // Port Number to Large
    int argc            = 3;
    char program_name[] = "TEST";
    char port_flag[]    = "-p";
    char port[]         = "70000";
    char* argv[]        = {program_name, port_flag, port};

    EXPECT_FALSE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

TEST(ServerTEST, MainUtilitiesServer_Negative_3_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    // Negative Argument
    int argc            = 3;
    char program_name[] = "TEST";
    char client_flag[]  = "-c";
    char max_clients[]  = "-1";
    char* argv[]        = {program_name, client_flag, max_clients};

    EXPECT_FALSE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

TEST(ServerTEST, MainUtilitiesServer_Negative_4_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    // Negative Argument
    int argc            = 3;
    char program_name[] = "TEST";
    char events_flag[]  = "-e";
    char max_events[]   = "-1";
    char* argv[]        = {program_name, events_flag, max_events};

    EXPECT_FALSE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

TEST(ServerTEST, MainUtilitiesServer_Negative_5_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    // Negative Argument
    int argc            = 3;
    char program_name[] = "TEST";
    char buffer_flag[]  = "-b";
    char buffer_size[]  = "-1";
    char* argv[]        = {program_name, buffer_flag, buffer_size};

    EXPECT_FALSE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

TEST(ServerTEST, MainUtilitiesServer_Negative_6_Test)
{
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    char TCP_UDP = 'T';

    // Incorrect Flag
    int argc            = 3;
    char program_name[] = "TEST";
    char buffer_flag[]  = "-r";
    char buffer_size[]  = "10";
    char* argv[]        = {program_name, buffer_flag, buffer_size};

    EXPECT_FALSE(dro::validateServerParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE, TCP_UDP));
}

// CLIENT UTILITIES
// ========================================================================================

TEST(ClientTEST, MainUtilitiesClient_Correct_1_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    int argc            = 1;
    char program_name[] = "TEST";
    char* argv[]        = {program_name};

    EXPECT_TRUE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
}

TEST(ClientTEST, MainUtilitiesClient_Correct_2_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    int argc            = 3;
    char program_name[] = "TEST";
    char port_flag[]    = "-p";
    char port[]         = "40000";
    char* argv[]        = {program_name, port_flag, port};

    EXPECT_TRUE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
    EXPECT_EQ(PORT, 40000);
}

TEST(ClientTEST, MainUtilitiesClient_Correct_3_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    int argc               = 3;
    char program_name[]    = "TEST";
    char ip_address_flag[] = "-i";
    char ip_address[]      = "127.0.0.5";
    char* argv[]           = {program_name, ip_address_flag, ip_address};

    EXPECT_TRUE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
    EXPECT_EQ(IP_ADDRESS, "127.0.0.5");
}

TEST(ClientTEST, MainUtilitiesClient_Correct_4_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    int argc            = 3;
    char program_name[] = "TEST";
    char buffer_flag[]  = "-b";
    char buffer_size[]  = "1000";
    char* argv[]        = {program_name, buffer_flag, buffer_size};

    EXPECT_TRUE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

TEST(ClientTEST, MainUtilitiesClient_Correct_5_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    int argc               = 7;
    char program_name[]    = "TEST";
    char port_flag[]       = "-p";
    char port[]            = "40000";
    char ip_address_flag[] = "-i";
    char ip_address[]      = "127.0.0.5";
    char buffer_flag[]     = "-b";
    char buffer_size[]     = "1000";
    char* argv[]           = {program_name, buffer_flag, buffer_size, ip_address_flag, ip_address, port_flag, port};

    EXPECT_TRUE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
    EXPECT_EQ(PORT, 40000);
    EXPECT_EQ(IP_ADDRESS, "127.0.0.5");
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

// ========================================================================================

TEST(ClientTEST, MainUtilitiesClient_Negative_1_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    // Argc Too Large
    int argc            = 12;
    char program_name[] = "TEST";
    char* argv[]        = {program_name};

    EXPECT_FALSE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
}

TEST(ClientTEST, MainUtilitiesClient_Negative_2_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    // Port Number to Large
    int argc            = 3;
    char program_name[] = "TEST";
    char port_flag[]    = "-p";
    char port[]         = "70000";
    char* argv[]        = {program_name, port_flag, port};

    EXPECT_FALSE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
}

TEST(ClientTEST, MainUtilitiesClient_Negative_3_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    // Incorrect IP Address
    int argc               = 3;
    char program_name[]    = "TEST";
    char ip_address_flag[] = "-i";
    char ip_address[]      = "127.0.0.0.0";
    char* argv[]           = {program_name, ip_address_flag, ip_address};

    EXPECT_FALSE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
}

TEST(ClientTEST, MainUtilitiesClient_Negative_4_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    // Negative Argument
    int argc            = 3;
    char program_name[] = "TEST";
    char buffer_flag[]  = "-b";
    char buffer_size[]  = "-1";
    char* argv[]        = {program_name, buffer_flag, buffer_size};

    EXPECT_FALSE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
}

TEST(ClientTEST, MainUtilitiesClient_Negative_5_Test)
{
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    char TCP_UDP   = 'T';
    bool BENCHMARK = true;

    // Incorrect Flag
    int argc            = 3;
    char program_name[] = "TEST";
    char port_flag[]    = "-r";
    char port[]         = "10";
    char* argv[]        = {program_name, port_flag, port};

    EXPECT_FALSE(dro::validateClientParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE, TCP_UDP, BENCHMARK));
}

}// namespace
