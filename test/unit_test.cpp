// Copyright 2024, Andrew Drogalis
// GNU License

#include <string>

#include "gtest/gtest.h"

#include "tcp-server.h"
#include "tcp-client.h"
#include "main-utilities-server.hpp"
#include "main-utilities-client.hpp"

namespace {

TEST(TCPServerTEST, Constructor) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;
    tcpserver::TCPServer tcpServ {PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE};
    
    SUCCEED();
}

TEST(TCPClientTEST, Constructor) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;
    tcpclient::TCPClient tcpClient {IP_ADDRESS, PORT, BUFFER_SIZE};
    
    SUCCEED();
}

// SERVER UTILITIES
// ========================================================================================

TEST(TCPServerTEST, MainUtilitiesServer_Correct_1_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    int argc = 1;
    char program_name[] = "TEST";
    char* argv[] = {program_name};
    
    EXPECT_TRUE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

TEST(TCPServerTEST, MainUtilitiesServer_Correct_2_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char port_flag[] = "-p";
    char port[] = "40000";
    char* argv[] = {program_name, port_flag, port};
    
    EXPECT_TRUE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
    EXPECT_EQ(PORT, 40000);
}

TEST(TCPServerTEST, MainUtilitiesServer_Correct_3_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char client_flag[] = "-c";
    char max_clients[] = "100";
    char* argv[] = {program_name, client_flag, max_clients};
    
    EXPECT_TRUE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
    EXPECT_EQ(MAX_CLIENTS, 100);
}

TEST(TCPServerTEST, MainUtilitiesServer_Correct_4_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char events_flag[] = "-e";
    char max_events[] = "101";
    char* argv[] = {program_name, events_flag, max_events};
    
    EXPECT_TRUE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
    EXPECT_EQ(MAX_EVENTS, 101);
}

TEST(TCPServerTEST, MainUtilitiesServer_Correct_5_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char buffer_flag[] = "-b";
    char buffer_size[] = "1000";
    char* argv[] = {program_name, buffer_flag, buffer_size};
    
    EXPECT_TRUE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

TEST(TCPServerTEST, MainUtilitiesServer_Correct_6_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    int argc = 9;
    char program_name[] = "TEST";
    char port_flag[] = "-p";
    char port[] = "40000";
    char client_flag[] = "-c";
    char max_clients[] = "100";
    char events_flag[] = "-e";
    char max_events[] = "101";
    char buffer_flag[] = "-b";
    char buffer_size[] = "1000";
    char* argv[] = {program_name, buffer_flag, buffer_size, events_flag, max_events, client_flag, max_clients, port_flag, port};
    
    EXPECT_TRUE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
    EXPECT_EQ(PORT, 40000);
    EXPECT_EQ(MAX_CLIENTS, 100);
    EXPECT_EQ(MAX_EVENTS, 101);
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

// ========================================================================================

TEST(TCPServerTEST, MainUtilitiesServer_Negative_1_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    // Argc Too Large
    int argc = 10;
    char program_name[] = "TEST";
    char* argv[] = {program_name};
    
    EXPECT_FALSE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

TEST(TCPServerTEST, MainUtilitiesServer_Negative_2_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    // Port Number to Large
    int argc = 3;
    char program_name[] = "TEST";
    char port_flag[] = "-p";
    char port[] = "70000";
    char* argv[] = {program_name, port_flag, port};
    
    EXPECT_FALSE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

TEST(TCPServerTEST, MainUtilitiesServer_Negative_3_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    // Negative Argument 
    int argc = 3;
    char program_name[] = "TEST";
    char client_flag[] = "-c";
    char max_clients[] = "-1";
    char* argv[] = {program_name, client_flag, max_clients};
    
    EXPECT_FALSE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

TEST(TCPServerTEST, MainUtilitiesServer_Negative_4_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    // Negative Argument 
    int argc = 3;
    char program_name[] = "TEST";
    char events_flag[] = "-e";
    char max_events[] = "-1";
    char* argv[] = {program_name, events_flag, max_events};
    
    EXPECT_FALSE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

TEST(TCPServerTEST, MainUtilitiesServer_Negative_5_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    // Negative Argument 
    int argc = 3;
    char program_name[] = "TEST";
    char buffer_flag[] = "-b";
    char buffer_size[] = "-1";
    char* argv[] = {program_name, buffer_flag, buffer_size};
    
    EXPECT_FALSE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

TEST(TCPServerTEST, MainUtilitiesServer_Negative_6_Test) {
    int PORT = 50000, MAX_CLIENTS = 10, MAX_EVENTS = 10, BUFFER_SIZE = 1024;

    // Incorrect Flag
    int argc = 3;
    char program_name[] = "TEST";
    char buffer_flag[] = "-r";
    char buffer_size[] = "10";
    char* argv[] = {program_name, buffer_flag, buffer_size};
    
    EXPECT_FALSE(tcpserver::validateMainParameters(argc, argv, PORT, MAX_CLIENTS, MAX_EVENTS, BUFFER_SIZE));
}

// CLIENT UTILITIES
// ========================================================================================

TEST(TCPClientTEST, MainUtilitiesClient_Correct_1_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    int argc = 1;
    char program_name[] = "TEST";
    char* argv[] = {program_name};
    
    EXPECT_TRUE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
}

TEST(TCPClientTEST, MainUtilitiesClient_Correct_2_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char port_flag[] = "-p";
    char port[] = "40000";
    char* argv[] = {program_name, port_flag, port};
    
    EXPECT_TRUE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
    EXPECT_EQ(PORT, 40000);
}

TEST(TCPClientTEST, MainUtilitiesClient_Correct_3_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char ip_address_flag[] = "-i";
    char ip_address[] = "127.0.0.5";
    char* argv[] = {program_name, ip_address_flag, ip_address};
    
    EXPECT_TRUE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
    EXPECT_EQ(IP_ADDRESS, "127.0.0.5");
}

TEST(TCPClientTEST, MainUtilitiesClient_Correct_4_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    int argc = 3;
    char program_name[] = "TEST";
    char buffer_flag[] = "-b";
    char buffer_size[] = "1000";
    char* argv[] = {program_name, buffer_flag, buffer_size};
    
    EXPECT_TRUE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

TEST(TCPClientTEST, MainUtilitiesClient_Correct_5_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    int argc = 7;
    char program_name[] = "TEST";
    char port_flag[] = "-p";
    char port[] = "40000";
    char ip_address_flag[] = "-i";
    char ip_address[] = "127.0.0.5";
    char buffer_flag[] = "-b";
    char buffer_size[] = "1000";
    char* argv[] = {program_name, buffer_flag, buffer_size, ip_address_flag, ip_address, port_flag, port};
    
    EXPECT_TRUE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
    EXPECT_EQ(PORT, 40000);
    EXPECT_EQ(IP_ADDRESS, "127.0.0.5");
    EXPECT_EQ(BUFFER_SIZE, 1000);
}

// ========================================================================================

TEST(TCPClientTEST, MainUtilitiesClient_Negative_1_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    // Argc Too Large
    int argc = 8;
    char program_name[] = "TEST";
    char* argv[] = {program_name};
    
    EXPECT_FALSE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
}

TEST(TCPClientTEST, MainUtilitiesClient_Negative_2_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    // Port Number to Large
    int argc = 3;
    char program_name[] = "TEST";
    char port_flag[] = "-p";
    char port[] = "70000";
    char* argv[] = {program_name, port_flag, port};
    
    EXPECT_FALSE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
}

TEST(TCPClientTEST, MainUtilitiesClient_Negative_3_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    // Incorrect IP Address 
    int argc = 3;
    char program_name[] = "TEST";
    char ip_address_flag[] = "-i";
    char ip_address[] = "127.0.0.0.0";
    char* argv[] = {program_name, ip_address_flag, ip_address};
    
    EXPECT_FALSE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
}

TEST(TCPClientTEST, MainUtilitiesClient_Negative_4_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    // Negative Argument 
    int argc = 3;
    char program_name[] = "TEST";
    char buffer_flag[] = "-b";
    char buffer_size[] = "-1";
    char* argv[] = {program_name, buffer_flag, buffer_size};
    
    EXPECT_FALSE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
}

TEST(TCPClientTEST, MainUtilitiesClient_Negative_5_Test) {
    std::string IP_ADDRESS = "127.0.0.1";
    int PORT = 50000, BUFFER_SIZE = 1024;

    // Incorrect Flag
    int argc = 3;
    char program_name[] = "TEST";
    char port_flag[] = "-r";
    char port[] = "10";
    char* argv[] = {program_name, port_flag, port};
    
    EXPECT_FALSE(tcpclient::validateMainParameters(argc, argv, IP_ADDRESS, PORT, BUFFER_SIZE));
}


} // namespace