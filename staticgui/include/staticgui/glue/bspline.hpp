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
#include <vector>

#include <staticgui/glue/simd.hpp>

namespace staticgui {
namespace glue {

    struct bspline {
        bspline(const std::vector<float>& strided_controls, const bool drop_invalid = true);
        bspline(const std::vector<simd_array<float, 2>>& controls, const bool drop_invalid = true);
        bspline(const float departure, const float arrival, const std::vector<float>& strided_controls = {}, const bool drop_invalid = true);
        bspline(const float departure, const float arrival, const std::vector<simd_array<float, 2>>& controls = {}, const bool drop_invalid = true);
        bspline(const bspline& other);
        bspline& operator=(const bspline& other);
        bspline(bspline&& other);
        bspline& operator=(bspline&& other);

        simd_array<float, 2> get_eval(const float t);
        std::vector<float> get_strided_samples(const unsigned int size);

    private:
        std::any _bspline_impl = nullptr;
    };
}
}
