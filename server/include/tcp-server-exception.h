// Copyright 2024, Andrew Drogalis
// GNU License

#ifndef TCP_SERVER_EXCEPTION_H
#define TCP_SERVER_EXCEPTION_H

#include <stdexcept>// for runtime_error
#include <string>   // for basic_string

namespace tcpserver
{

class TCPServerException : public std::runtime_error
{
  public:
    std::string message;
    std::string func_name;

    TCPServerException() = delete;

    TCPServerException(std::string const& func_name, std::string const& message);

    char const* what() const noexcept override;

    char const* where() const noexcept;
};

}// namespace tcpserver

#endif
