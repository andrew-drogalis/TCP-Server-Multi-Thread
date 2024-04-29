// Copyright 2024, Andrew Drogalis
// GNU License

#ifndef TCP_CLIENT_EXCEPTION_H
#define TCP_CLIENT_EXCEPTION_H

#include <stdexcept>// for runtime_error
#include <string>   // for basic_string

namespace tcpclient
{

class TCPClientException : public std::runtime_error
{
  public:
    std::string message;
    std::string func_name;

    TCPClientException() = delete;

    TCPClientException(std::string const& func_name, std::string const& message);

    char const* what() const noexcept override;

    char const* where() const noexcept;
};

}// namespace tcpclient

#endif
