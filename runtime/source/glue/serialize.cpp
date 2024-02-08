#include <sstream>

#include <bungeegum/glue/serialize.hpp>

namespace bungeegum {
namespace detail {

    std::vector<std::string> split_names(const std::string& comma_separated_names)
    {
        std::vector<std::string> _names;
        std::stringstream _sstream(comma_separated_names);
        while (_sstream.good()) {
            std::string _substr;
            std::getline(_sstream, _substr, ',');
            if (_substr[0] == ' ') {
                _substr = _substr.substr(1, _substr.length() - 1);
            }
            _names.push_back(_substr);
        }
        return _names;
    }

}
}