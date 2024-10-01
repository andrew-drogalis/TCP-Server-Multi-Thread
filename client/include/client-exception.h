// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_CLIENT_EXCEPTION_H
#define DRO_CLIENT_EXCEPTION_H

#include <stdexcept>// for runtime_error
#include <string>   // for basic_string

namespace dro
{

class ClientException : public std::runtime_error
{
  public:
    std::string message;
    std::string func_name;

    ClientException() = delete;

    ClientException(std::string const& func_name, std::string const& message);

    char const* what() const noexcept override;

    char const* where() const noexcept;
};

}// namespace dro
#endif
