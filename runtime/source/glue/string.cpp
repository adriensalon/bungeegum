#include <bungeegum/glue/string.hpp>

#include <locale>
#include <sstream>

namespace bungeegum {
namespace detail {

    bool contains(const std::string& str, const std::string& regex_str)
    {
        std::regex _regex(regex_str, std::regex_constants::ECMAScript);
        return std::regex_search(str, _regex);
    }

    bool contains(const std::string& str, const std::regex& regex)
    {
        return std::regex_search(str, regex);
    }
    
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

    std::string replace(const std::string& str, const std::string& to_replace, const std::string& replace_with)
    {
        std::size_t _pos = 0;
        std::size_t _cursor = 0;
        std::size_t _rep_len = to_replace.length();
        std::stringstream _builder;
        do {
            _pos = str.find(to_replace, _cursor);
            if (std::string::npos != _pos) {
                _builder << str.substr(_cursor, _pos - _cursor);
                _builder << replace_with;
                _cursor = _pos + _rep_len;
            }
        } while (std::string::npos != _pos);
        _builder << str.substr(_cursor);
        return _builder.str();
    }

    std::vector<std::string> split(const std::string& str, const char sep)
    {
        std::vector<std::string> _retval;
        std::stringstream _builder(str);
        while (_builder.good()) {
            std::string _substr;
            std::getline(_builder, _substr, sep);
            if (_substr[0] == ' ') {
                _substr = _substr.substr(1, _substr.length() - 1);
            }
            _retval.push_back(_substr);
        }
        return _retval;
    }
}
}