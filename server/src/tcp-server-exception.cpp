// Copyright 2024, Andrew Drogalis
// GNU License

#include <tcp-server-exception.h>

#include <stdexcept>// for runtime_error
#include <string>   // for basic_string

namespace tcpserver
{

TCPServerException::TCPServerException(std::string const& func_name, std::string const& message)
    : std::runtime_error {message}, func_name(func_name), message(message)
{
}

char const* TCPServerException::what() const noexcept { return message.c_str(); }

char const* TCPServerException::where() const noexcept { return func_name.c_str(); }

}// namespace tcpserver
