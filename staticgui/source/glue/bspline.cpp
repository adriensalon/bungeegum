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

#include <staticgui/glue/bspline.hpp>

namespace staticgui {
namespace detail {

    namespace impl {
        tinyspline::BSpline& get_bspline_impl(std::any& untyped)
        {
            return std::any_cast<tinyspline::BSpline&>(untyped);
        }
    }

    bspline::bspline(const std::vector<float>& strided_controls, const bool drop_invalid)
    {
        _bspline_impl = tinyspline::BSpline::interpolateCubicNatural(strided_controls, 2);
    }

    bspline::bspline(const std::vector<simd_array<float, 2>>& controls, const bool drop_invalid)
    {
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
        // _bspline_impl = tinyspline::BSpline::interpolateCubicNatural(_data, 2);
    }

    bspline::bspline(const float departure, const float arrival, const std::vector<float>& strided_controls, const bool drop_invalid)
    {
        // TODO
    }

    bspline::bspline(const float departure, const float arrival, const std::vector<simd_array<float, 2>>& controls, const bool drop_invalid)
    {
        // VERIFY HEIN
        size_t _size = controls.size() + 2U;
        std::vector<float> _data(2 * _size);
        _data[0] = 0.f;
        _data[1] = departure;
        _data[2 * _size - 2] = 1.f;
        _data[2 * _size - 1] = arrival;
        for (size_t _k = 2; _k < 2 * _size - 3; _k += 2) {
            _data[_k] = controls[_k / 2 - 1].x();
            _data[_k + 1] = controls[_k / 2 - 1].y();
        }
        _bspline_impl = tinyspline::BSpline::interpolateCubicNatural(_data, 2);
    }

    bspline::bspline(const bspline& other)
    {
        *this = other;
    }

    bspline& bspline::operator=(const bspline& other)
    {
        _bspline_impl = other._bspline_impl;
        return *this;
    }

    bspline::bspline(bspline&& other)
    {
        *this = std::move(other);
    }

    bspline& bspline::operator=(bspline&& other)
    {
        _bspline_impl = std::move(other._bspline_impl);
        return *this;
    }

    simd_array<float, 2> bspline::get_eval(const float t)
    {
        float _clamped = std::fmin(1.f, std::fmax(0.f, t));
        auto& _vector = impl::get_bspline_impl(_bspline_impl).eval(_clamped).result();
        return std::array<float, 2> { _vector[0], _vector[1] };
    }

    std::vector<float> bspline::get_strided_samples(const unsigned int size)
    {
        return impl::get_bspline_impl(_bspline_impl).sample(size);
    }
}
}
