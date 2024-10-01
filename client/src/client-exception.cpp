// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <client-exception.h>

#include <stdexcept>// for runtime_error
#include <string>   // for basic_string

namespace dro
{

ClientException::ClientException(std::string const& func_name, std::string const& message)
    : std::runtime_error {message}, func_name(func_name), message(message)
{
}

char const* ClientException::what() const noexcept { return message.c_str(); }

char const* ClientException::where() const noexcept { return func_name.c_str(); }

}// namespace dro
