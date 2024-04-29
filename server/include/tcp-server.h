// Copyright 2024, Andrew Drogalis
// GNU License

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>// for basic_string

namespace tcpserver
{

class TCPServer
{

  public:
    TCPServer() = default;

    TCPServer(int PORT, int max_events, int max_clients);

    void start_server();

    void* handle_client_request(std::string client_request);

  private:
    int PORT, max_events, max_clients;
};

}// namespace tcpserver

#endif
