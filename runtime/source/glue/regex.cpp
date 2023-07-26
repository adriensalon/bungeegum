#include <bungeegum/glue/regex.hpp>

namespace bungeegum {
namespace detail {

    bool regex_search(const std::string& str, const std::string& regex_str)
    {
        std::regex _regex(regex_str, std::regex_constants::ECMAScript);
        return std::regex_search(str, _regex);
    }

    bool regex_search(const std::string& str, const std::regex& regex)
    {
        return std::regex_search(str, regex);
    }
}
}