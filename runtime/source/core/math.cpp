#include <bungeegum/core/math.hpp>

namespace bungeegum { 
    
curve::curve(const curve_preset& preset)
{
    (void)preset;
    // switch (preset)
    // {
    // case /* constant-expression */:
    //     /* code */
        
    //     return curve({ { 0.f, 0.f }, { 1.f, 1.f } });
    //     break;
    
    // default:
    //     break;
    // }
}

curve::curve(const std::vector<float>& strided_controls)
{
    _data = tinyspline::BSpline::interpolateCubicNatural(strided_controls, 2);
}

curve::curve(const std::vector<float2>& controls)
{
    std::size_t _half_size = controls.size();
    std::vector<float> _controls_data(2 * _half_size);
    for (std::size_t _k = 0; _k < 2 * _half_size; _k += 2) {
        _controls_data[_k] = controls[_k / 2].x;
        _controls_data[_k + 1] = controls[_k / 2].y;
    }
    _data = tinyspline::BSpline::interpolateCubicNatural(_controls_data, 2);
}

curve::curve(const float departure, const float arrival, const std::vector<float>& strided_controls)
{
    std::size_t _total_size = strided_controls.size() + 4;
    std::vector<float> _controls_data(_total_size);
    _controls_data[0] = 0.f;
    _controls_data[1] = departure;
    _controls_data[_total_size - 2] = 1.f;
    _controls_data[_total_size - 1] = arrival;
    _controls_data.insert(_controls_data.begin() + 2,
        strided_controls.begin(),
        strided_controls.end());
    _data = tinyspline::BSpline::interpolateCubicNatural(_controls_data, 2);
}

curve::curve(const float departure, const float arrival, const std::vector<float2>& controls)
{
    std::size_t _half_size = controls.size() + 2;
    std::vector<float> _controls_data(2 * _half_size);
    _controls_data[0] = 0.f;
    _controls_data[1] = departure;
    _controls_data[2 * _half_size - 2] = 1.f;
    _controls_data[2 * _half_size - 1] = arrival;
    for (std::size_t _k = 2; _k < 2 * _half_size - 3; _k += 2) {
        _controls_data[_k] = controls[_k / 2 - 1].x;
        _controls_data[_k + 1] = controls[_k / 2 - 1].y;
    }
    _data = tinyspline::BSpline::interpolateCubicNatural(_controls_data, 2);
}

// curve curve::linear()
// {
//     return curve({ { 0.f, 0.f }, { 1.f, 1.f } });
// }

// curve curve::bounce_in()
// {
//     return curve(
//         {
//             { 0.f, 0.f },
//             { 0.5f, 0.8f },
//             { 0.8f, 0.6f },
//             { 1.f, 1.f },
//         }); // TODOOOOO
// }

float curve::evaluate_1d(const float fraction)
{
    float _clamped = math::clamp(fraction, 0.f, 1.f);
    const std::vector<float>& _result_data = _data.bisect(_clamped).result();
    return _result_data[1];
}

float2 curve::evaluate_2d(const float fraction)
{
    float _clamped = math::clamp(fraction, 0.f, 1.f);
    const std::vector<float>& _result_data = _data.eval(_clamped).result();
    return { _result_data[0], _result_data[1] };
}

std::vector<float> curve::get_strided_samples(const std::size_t count)
{
    return (_data.sample(count));
}

}