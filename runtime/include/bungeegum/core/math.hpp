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
constexpr inline static value_t zero = static_cast<value_t>(0.0);

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