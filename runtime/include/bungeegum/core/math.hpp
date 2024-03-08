#pragma once

#include <bungeegum/core/math.fwd>

namespace bungeegum {

/// @brief
using float2 = detail::simd_vector<float, 2>;

/// @brief
using float3 = detail::simd_vector<float, 3>;

/// @brief
using float4 = detail::simd_vector<float, 4>;

/// @brief
using float3x3 = detail::simd_matrix<float, 3>;

/// @brief
using float4x4 = detail::simd_matrix<float, 4>;

/// @brief
using int2 = detail::simd_vector<int, 2>;

/// @brief
using int3 = detail::simd_vector<int, 3>;

/// @brief
using int4 = detail::simd_vector<int, 4>;

/// @brief
using int3x3 = detail::simd_matrix<int, 3>;

/// @brief
using int4x4 = detail::simd_matrix<int, 4>;

/// @brief
using uint2 = detail::simd_vector<unsigned int, 2>;

/// @brief
using uint3 = detail::simd_vector<unsigned int, 3>;

/// @brief
using uint4 = detail::simd_vector<unsigned int, 4>;

/// @brief
using uint3x3 = detail::simd_matrix<unsigned int, 3>;

/// @brief
using uint4x4 = detail::simd_matrix<unsigned int, 4>;

/// @brief
using bool2 = detail::simd_vector<bool, 2>;

/// @brief
using bool3 = detail::simd_vector<bool, 3>;

/// @brief
using bool4 = detail::simd_vector<bool, 4>;

/// @brief
using bool3x3 = detail::simd_matrix<bool, 3>;

/// @brief
using bool4x4 = detail::simd_matrix<bool, 4>;

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr inline static value_t zero = static_cast<value_t>(0);

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr inline static value_t infinity = std::numeric_limits<value_t>::max();

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr inline static value_t identity = "value_t must be a matrix type";

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr bool is_zero(const value_t value);

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr bool is_infinite(const value_t value);

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr bool is_finite(const value_t value);

/// @brief
/// @tparam value_t
template <typename value_t>
std::string to_string(const value_t& value);

/// @brief 
enum struct curve_preset {
	linear,
	bounce_in
};

/// @brief Instance of this struct represent immutable 2D spline curves that can be evaluated
/// and sampled after they have been created from points and optional departure (0, d) and arrival
/// (1, a) values.
/// @details Instances of this struct can be copied (performing deep copy) and moved.
struct curve {

    /// @brief
	curve() = default;

    /// @brief
    /// @param other
	curve(const curve& other) = default;

    /// @brief
    /// @param other
	curve& operator=(const curve& other) = default;

    /// @brief
    /// @param other
	curve(curve&& other) = default;

    /// @brief
    /// @param other
	curve& operator=(curve&& other) = default;

	/// @brief
	/// @param preset
	curve(const curve_preset& preset);

	/// @brief Creates an instance from strided control points.
	curve(const std::vector<float>& strided_controls);

	/// @brief Creates an instance from control points.
	curve(const std::vector<float2>& controls);

	/// @brief Creates an instance from strided control points and departure and arrival values.
	curve(const float departure, const float arrival, const std::vector<float>& strided_controls);

	/// @brief Creates an instance from control points and departure and arrival values.
	curve(const float departure, const float arrival, const std::vector<float2>& controls);

	/// @brief
	/// @param preset
	curve& emplace(const curve_preset& preset);

	/// @brief 
	/// @param controls 
	curve& emplace(const std::vector<float2>& controls);

	/// @brief 
	/// @param departure 
	/// @param arrival 
	/// @param controls 
	curve& emplace(const float departure, const float arrival, const std::vector<float2>& controls);

	/// @brief 
	/// @param departure 
	/// @param arrival 
	/// @param strided_controls 
	curve& emplace(const float departure, const float arrival, const std::vector<float>& strided_controls);

	/// @brief Evaluates the curve at the given fraction.
	/// @details Clamps fraction between 0.f and 1.f.
	float evaluate_1d(const float fraction);

	/// @brief Evaluates the curve at the given fraction.
	/// @details Clamps fraction between 0.f and 1.f.
	float2 evaluate_2d(const float fraction);

	/// @brief Samples the curve given a sample count.
	std::vector<float> get_strided_samples(const std::size_t count);

private:
	detail::curve_data _data;
};

namespace math {

    using namespace glm;	

	/// @brief 
	/// @param position 
	/// @param rotation 
	/// @param scale 
	/// @return 
	float4x4 transform(const float3 position, const float3 rotation, const float3 scale);

    /// @brief Linear interpolation between 2 values.
    /// @tparam value_t is a type that can be added to another value_t and multiplied by a float.
    template <typename value_t>
    value_t lerp(value_t& min_value, value_t& max_value, const float t);

}

}

#include <bungeegum/core/math.inl>