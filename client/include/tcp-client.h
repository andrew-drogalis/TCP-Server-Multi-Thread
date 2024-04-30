// Copyright 2024, Andrew Drogalis
// GNU License

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <expected>// for expected

#include "tcp-client-exception.h"// for TCPClientException

namespace tcpclient
{

class TCPClient
{

  public:
    TCPClient() = default;

    TCPClient(struct hostent* server_ip, int port, int buffer_size);

    std::expected<bool, TCPClientException> start_client();

    void clean_up_socket(int socket_FD);

        private : struct hostent* server_ip;
    int port, buffer_size;
};

}// namespace tcpclient

#endif
