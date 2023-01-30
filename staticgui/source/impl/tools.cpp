//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace tools {

    any_function::any_function() noexcept
    {
        _untyped = nullptr;
        _typeindex = nullptr;
    }

    any_function::any_function(const any_function& other)
    {
        *this = other;
    }

    any_function& any_function::operator=(const any_function& other)
    {
        _untyped = other._untyped;
        _typeindex = other._typeindex;
        return *this;
    }

    any_function::any_function(any_function&& other) noexcept
    {
        *this = std::move(other);
    }

    any_function& any_function::operator=(any_function&& other) noexcept
    {
        _untyped = std::move(other._untyped);
        _typeindex = std::move(other._typeindex);
        return *this;
    }

}
}