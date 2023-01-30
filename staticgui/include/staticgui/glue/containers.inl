//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <variant>

namespace staticgui {
namespace glue {
    namespace containers {

        template <typename return_t, typename... args_t>
        any_function& any_function::operator=(const std::function<return_t(args_t...)>& function)
        {
            _untyped = std::make_any<std::function<return_t(args_t...)>>(function);
            return *this;
        }

        template <typename return_t, typename... args_t>
        std::function<return_t(args_t...)>& any_function::get()
        {
            return std::any_cast<std::function<return_t(args_t...)>&>(_untyped);
        }

        template <typename return_t, typename... args_t>
        return_t any_function::operator()(args_t&&... args)
        {
            auto& _typed = std::any_cast<std::function<return_t(args_t...)>&>(_untyped);
            return _typed(std::forward<args_t>(args)...);
        }

    }
}
}