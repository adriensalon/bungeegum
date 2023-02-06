//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <vector>

#include <staticgui/glue/simd.hpp>

namespace staticgui {
namespace detail {

    struct curve_data {

        float& departure();
        const float& departure() const;
        float& arrival();
        const float& arrival() const;
        std::vector<glue::simd_array<float, 2>>& control_points();
        const std::vector<glue::simd_array<float, 2>>& control_points() const;

        float evaluate(const float t) const;
        unsigned int size() const;

    private:
        bool _is_dirty = false;
        float _first_control_point_y = 0.f;
        float _last_control_point_y = 0.f;
        std::vector<glue::simd_array<float, 2>> _control_points;
    };

}
}

#include <staticgui/state/curve.inl>
