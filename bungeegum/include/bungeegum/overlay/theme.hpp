//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <bungeegum/context/context.hpp>

namespace bungeegum {
namespace detail {

    constexpr inline float footer_height = 28.f;

    inline static ImFont* regular_font = nullptr;
    inline static ImFont* extrabold_font = nullptr;
    inline static ImFont* icons_font = nullptr;

    void setup_overlay_theme();

}
}
