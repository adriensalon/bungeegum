#pragma once

#include <any>
#include <vector>

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {

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
