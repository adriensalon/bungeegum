//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <memory>
#include <vector>

#include <staticgui/glue/bspline.hpp>
#include <staticgui/glue/simd.hpp>

namespace staticgui {
namespace detail {

    struct curve_data {
        std::unique_ptr<glue::bspline> spline = nullptr;
    };

}
}
