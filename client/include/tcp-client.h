// Copyright 2024, Andrew Drogalis
// GNU License

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

namespace tcpclient
{

class TCPClient
{

  public:
    TCPClient() = delete;

    TCPClient(int port, int buffer_size, struct hostent* server_ip);

    bool start_client();

  private:
    int port, buffer_size;
    struct hostent* server_ip;
};

}// namespace tcpclient

#endif
