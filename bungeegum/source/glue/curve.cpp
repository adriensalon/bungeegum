//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <cmath>
#include <iostream>

#include <tinysplinecxx.h>

#include <bungeegum/glue/curve.hpp>

namespace bungeegum {

namespace {
    tinyspline::BSpline& get_bspline_impl(std::any& untyped)
    {
        return std::any_cast<tinyspline::BSpline&>(untyped);
    }
}

curve::curve(const std::vector<float>& strided_controls, const bool drop_invalid)
{
    (void)drop_invalid;
    _tinyspline_curve = tinyspline::BSpline::interpolateCubicNatural(strided_controls, 2);
}

curve::curve(const std::vector<float2>& controls, const bool drop_invalid)
{
    (void)controls;
    (void)drop_invalid;
    // TODO
    // size_t _size = controls.size();
    // std::vector<float> _data(_size);
    // _data[0] = 0.f;
    // _data[1] = departure;
    // _data[_size - 2] = 0.f;
    // _data[_size - 1] = arrival;
    // for (size_t _k = 2; _k < _size - 3; _k += 2) {
    //     _data[_k] = controls[_k - 1].x();
    //     _data[_k + 1] = controls[_k - 1].y();
    // }
    // _tinyspline_curve = tinyspline::BSpline::interpolateCubicNatural(_data, 2);
}

curve::curve(const float departure, const float arrival, const std::vector<float>& strided_controls, const bool drop_invalid)
{
    (void)departure;
    (void)arrival;
    (void)strided_controls;
    (void)drop_invalid;
    // TODO
}

curve::curve(const float departure, const float arrival, const std::vector<float2>& controls, const bool drop_invalid)
{
    (void)drop_invalid;
    // VERIFY HEIN
    size_t _size
        = controls.size() + 2U;
    std::vector<float> _data(2 * _size);
    _data[0] = 0.f;
    _data[1] = departure;
    _data[2 * _size - 2] = 1.f;
    _data[2 * _size - 1] = arrival;
    for (size_t _k = 2; _k < 2 * _size - 3; _k += 2) {
        _data[_k] = controls[_k / 2 - 1].x;
        _data[_k + 1] = controls[_k / 2 - 1].y;
    }
    _tinyspline_curve = tinyspline::BSpline::interpolateCubicNatural(_data, 2);
}

curve::curve(const curve& other)
{
    *this = other;
}

curve& curve::operator=(const curve& other)
{
    _tinyspline_curve = other._tinyspline_curve;
    return *this;
}

curve::curve(curve&& other)
{
    *this = std::move(other);
}

curve& curve::operator=(curve&& other)
{
    _tinyspline_curve = std::move(other._tinyspline_curve);
    return *this;
}

float2 curve::eval(const float t)
{
    float _clamped = std::fmin(1.f, std::fmax(0.f, t));
    auto _vector = get_bspline_impl(_tinyspline_curve).eval(_clamped).result();
    return { _vector[0], _vector[1] };
}

std::vector<float> curve::strided_samples(const unsigned int size)
{
    return get_bspline_impl(_tinyspline_curve).sample(size);
}
}
