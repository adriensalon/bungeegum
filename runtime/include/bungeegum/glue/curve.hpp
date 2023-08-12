#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {

/// @brief
enum struct curve_preset {
    linear,
    bounce_in,
};

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
    curve(const std::vector<float1>& strided_controls);

    /// @brief Creates an instance from control points.
    curve(const std::vector<float2>& controls);

    /// @brief Creates an instance from strided control points and departure and arrival values.
    curve(const float1 departure, const float1 arrival, const std::vector<float1>& strided_controls);

    /// @brief Creates an instance from control points and departure and arrival values.
    curve(const float1 departure, const float1 arrival, const std::vector<float2>& controls);

    /// @brief Evaluates the curve at the given fraction.
    /// @details Clamps fraction between 0.f and 1.f.
    float2 evaluate(const float1 fraction);

    /// @brief Samples the curve given a sample count.
    std::vector<float1> strided_samples(const std::size_t count);

private:
    struct curve_data;
    std::shared_ptr<curve_data> _data = nullptr;
};
}
