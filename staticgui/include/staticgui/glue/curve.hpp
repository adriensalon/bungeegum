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

enum struct curve_preset {
    linear,
    bounce_in,
};

struct curve {
    curve(const std::vector<float>& strided_controls, const bool drop_invalid = true);
    curve(const std::vector<float2>& controls, const bool drop_invalid = true);
    curve(const float departure, const float arrival, const std::vector<float>& strided_controls = {}, const bool drop_invalid = true);
    curve(const float departure, const float arrival, const std::vector<float2>& controls = {}, const bool drop_invalid = true);
    curve(const curve& other);
    curve& operator=(const curve& other);
    curve(curve&& other);
    curve& operator=(curve&& other);

    float2 eval(const float t);
    std::vector<float> strided_samples(const unsigned int size);

private:
    std::any _tinyspline_curve = nullptr;
};

}
