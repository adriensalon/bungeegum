#pragma once

#include <bungeegum/core/curve.fwd>

namespace bungeegum {

/// @brief Instance of this struct represent immutable 2D spline curves that can be evaluated
/// and sampled after they have been created from points and optional departure (0, d) and arrival
/// (1, a) values.
/// @details Instances of this struct can be copied (performing deep copy) and moved.
struct curve {
    curve() = delete;
    curve(const curve& other);
    curve& operator=(const curve& other);
    curve(curve&& other) = default;
    curve& operator=(curve&& other) = default;

    /// @brief Creates an instance from strided control points.
    curve(const std::vector<float>& strided_controls);

    /// @brief Creates an instance from control points.
    curve(const std::vector<float2>& controls);

    /// @brief Creates an instance from strided control points and departure and arrival values.
    curve(const float departure, const float arrival, const std::vector<float>& strided_controls);

    /// @brief Creates an instance from control points and departure and arrival values.
    curve(const float departure, const float arrival, const std::vector<float2>& controls);

    /// @brief Creates an instance from the linear preset.
    static curve linear();

    /// @brief Creates an instance from the bounce_in preset.
    static curve bounce_in();

    // ETC

    /// @brief Evaluates the curve at the given fraction.
    /// @details Clamps fraction between 0.f and 1.f.
    float evaluate_1d(const float fraction);

    /// @brief Evaluates the curve at the given fraction.
    /// @details Clamps fraction between 0.f and 1.f.
    float2 evaluate_2d(const float fraction);

    /// @brief Samples the curve given a sample count.
    std::vector<float> get_strided_samples(const std::size_t count);

private:
    struct curve_data;
    std::shared_ptr<curve_data> _data = nullptr;
};

}
