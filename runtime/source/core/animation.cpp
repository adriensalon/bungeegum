#include <bungeegum/core/animation.hpp>

namespace bungeegum {
namespace detail {

    animations_manager::const_iterator animations_manager::begin() const
    {
        return _updatables.begin();
    }

    bool animations_manager::contains(const std::uintptr_t raw_animation) const
    {
        return _updatables.find(raw_animation) != _updatables.end();
    }

    animations_manager::const_iterator animations_manager::end() const
    {
        return _updatables.end();
    }

    void animations_manager::notify_erase(const std::uintptr_t raw_animation)
    {
        _updatables_to_erase.push_back(raw_animation);
    }

    animation_update_data& animations_manager::operator[](const std::uintptr_t raw_animation)
    {
        return _updatables[raw_animation];
    }

    std::size_t animations_manager::size() const
    {
        return _updatables.size();
    }

    void animations_manager::update(const std::chrono::milliseconds& delta_time)
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t& _raw_animation : _updatables_to_erase) {
            _updatables.erase(_raw_animation);
        }
        _updatables_to_erase.clear();
        for (std::pair<const uintptr_t, bungeegum::detail::animation_update_data>& _update_data : _updatables) {
            _update_data.second.ticker(delta_time);
        }
    }
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

curve curve::linear()
{
    return curve({ { 0.f, 0.f }, { 1.f, 1.f } });
}

curve curve::bounce_in()
{
    return curve(
        {
            { 0.f, 0.f },
            { 0.5f, 0.8f },
            { 0.8f, 0.6f },
            { 1.f, 1.f },
        }); // TODOOOOO
}

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