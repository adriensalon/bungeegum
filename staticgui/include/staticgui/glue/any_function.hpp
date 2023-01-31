//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <functional>
#include <memory>
#include <typeindex>

namespace staticgui {
namespace glue {

    struct any_function {
        any_function() noexcept;
        any_function(nullptr_t) noexcept;
        any_function(const any_function& other);
        any_function& operator=(const any_function& other);
        any_function(any_function&& other) noexcept;
        any_function& operator=(any_function&& other) noexcept;

        template <typename return_t, typename... args_t>
        any_function& operator=(const std::function<return_t(args_t...)>& function);

        template <typename return_t, typename... args_t>
        std::function<return_t(args_t...)>& get();

        template <typename return_t, typename... args_t>
        return_t operator()(args_t&&... args);

    private:
        std::any _untyped;
        std::shared_ptr<std::type_index> _typeindex;
    };

}
}

#include <staticgui/glue/any_function.inl>