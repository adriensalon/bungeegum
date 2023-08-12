#pragma once

#include <regex>
#include <string>

namespace bungeegum {
namespace detail {

    bool regex_search(const std::string& str, const std::string& regex_str);

    bool regex_search(const std::string& str, const std::regex& regex);

}
}