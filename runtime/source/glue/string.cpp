#include <bungeegum/glue/string.hpp>

#include <locale>

namespace bungeegum {
namespace detail {
    
    std::wstring widen(const std::string& str)
    {
        std::vector<wchar_t> _buffer(str.size());
        std::use_facet<std::ctype<wchar_t>>(std::locale()).widen(str.data(), str.data() + str.size(), _buffer.data());
        return std::wstring(_buffer.data(), _buffer.size());
    }

    std::string narrow(const std::wstring& wstr)
    {
        std::vector<char> _buffer(wstr.size());
        std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(wstr.data(), wstr.data() + wstr.size(), '0', _buffer.data());
        return std::string(_buffer.data(), _buffer.size());
    }

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