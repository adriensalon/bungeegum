#pragma once

#include <tinysplinecxx.h>

#include <bungeegum/glue/curve.hpp>

namespace bungeegum {

struct curve::curve_data {
    tinyspline::BSpline bspline;
};

curve::curve(const curve& other)
{
    _data = std::make_shared<curve_data>();
    *this = other;
}

curve& curve::operator=(const curve& other)
{
    _data->bspline = other._data->bspline;
    return *this;
}

curve::curve(const std::vector<float1>& strided_controls)
{
    _data = std::make_shared<curve_data>();
    _data->bspline = tinyspline::BSpline::interpolateCubicNatural(strided_controls, 2);
}

curve::curve(const std::vector<float2>& controls)
{
    _data = std::make_shared<curve_data>();
    std::size_t _half_size = controls.size();
    std::vector<float1> _controls_data(2 * _half_size);
    for (std::size_t _k = 0; _k < 2 * _half_size; _k += 2) {
        _controls_data[_k] = controls[_k / 2].x;
        _controls_data[_k + 1] = controls[_k / 2].y;
    }
    _data->bspline = tinyspline::BSpline::interpolateCubicNatural(_controls_data, 2);
}

curve::curve(const float1 departure, const float1 arrival, const std::vector<float1>& strided_controls)
{
    _data = std::make_shared<curve_data>();
    std::size_t _total_size = strided_controls.size() + 4;
    std::vector<float1> _controls_data(_total_size);
    _controls_data[0] = 0.f;
    _controls_data[1] = departure;
    _controls_data[_total_size - 2] = 1.f;
    _controls_data[_total_size - 1] = arrival;
    _controls_data.insert(_controls_data.begin() + 2,
        strided_controls.begin(),
        strided_controls.end());
    _data->bspline = tinyspline::BSpline::interpolateCubicNatural(_controls_data, 2);
}

curve::curve(const float1 departure, const float1 arrival, const std::vector<float2>& controls)
{
    _data = std::make_shared<curve_data>();
    std::size_t _half_size = controls.size() + 2;
    std::vector<float1> _controls_data(2 * _half_size);
    _controls_data[0] = 0.f;
    _controls_data[1] = departure;
    _controls_data[2 * _half_size - 2] = 1.f;
    _controls_data[2 * _half_size - 1] = arrival;
    for (std::size_t _k = 2; _k < 2 * _half_size - 3; _k += 2) {
        _controls_data[_k] = controls[_k / 2 - 1].x;
        _controls_data[_k + 1] = controls[_k / 2 - 1].y;
    }
    _data->bspline = tinyspline::BSpline::interpolateCubicNatural(_controls_data, 2);
}

float2 curve::evaluate(const float1 fraction)
{
    float1 _clamped = math::min(1.f, math::max(0.f, fraction));
    const std::vector<float1>& _result_data = _data->bspline.eval(_clamped).result();
    return { _result_data[0], _result_data[1] };
}

std::vector<float1> curve::strided_samples(const std::size_t count)
{
    return (_data->bspline.sample(count));
}
}
