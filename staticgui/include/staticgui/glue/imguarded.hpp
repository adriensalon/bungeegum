//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/simd.hpp>

namespace staticgui {
namespace glue {

    // struct font_guard {
    //     font_guard();
    //     font_guard(const font_guard& other) = delete;
    //     font_guard(const font& value);
    //     ~font_guard();

    // private:
    //     bool _is_set = false;
    // };

    struct item_guard {
        item_guard();
        item_guard(const item_guard& other) = delete;
        item_guard(const int target, const bool value);
        ~item_guard();

        void set(const int target, const bool value);

    private:
        bool _is_set = false;
    };

    struct color_guard {
        color_guard();
        color_guard(const color_guard& other) = delete;
        color_guard(const int& target, const simd_array<float, 4>& value);
        ~color_guard();

        void set(const int& target, const simd_array<float, 4>& value);

    private:
        bool _is_set = false;
    };

    struct style_guard {
        style_guard();
        style_guard(const style_guard& other) = delete;
        style_guard(const int& target, const float value);
        style_guard(const int& target, const simd_array<float, 2>& value);
        ~style_guard();

        void set(const int& target, const float value);
        void set(const int& target, const simd_array<float, 2>& value);

    private:
        bool _is_set = false;
    };

}
}
