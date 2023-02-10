//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>

#include <staticgui/glue/simd.hpp>

namespace staticgui {
namespace detail {

    // implements https://api.flutter.dev/flutter/rendering/RenderObject/parentData.html
    struct constraint_data {
        bool is_tight;
        bool has_infinite_width;
        bool has_infinite_height;
        simd_array<float, 2> min_size;
        simd_array<float, 2> max_size;
    };

    struct advice_data {
        simd_array<float, 2> size;
    };

    using resolve_function = std::function<void(const constraint_data&, advice_data&)>;
}
}
