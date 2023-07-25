#pragma once

#include <limits>
#include <string>

#include <glm/glm.hpp>

/// @brief
#if !defined(BUNGEEGUM_USE_SIMD_PRECISION)
#define BUNGEEGUM_USE_SIMD_PRECISION glm::packed_highp
#endif

namespace bungeegum {
namespace detail {

    template <typename value_t, std::size_t count_t>
    using simd_vector = glm::vec<count_t, value_t, BUNGEEGUM_USE_SIMD_PRECISION>;

    template <typename value_t, std::size_t count_t>
    using simd_matrix = glm::mat<count_t, count_t, value_t, BUNGEEGUM_USE_SIMD_PRECISION>;
}

/// @brief
namespace math = glm;

/// @brief
using float1 = float;

/// @brief
using int1 = int;

/// @brief
using uint1 = unsigned int;

/// @brief
using bool1 = bool;

/// @brief
using float2 = detail::simd_vector<float1, 2>;

/// @brief
using float3 = detail::simd_vector<float1, 3>;

/// @brief
using float4 = detail::simd_vector<float1, 4>;

/// @brief
using float3x3 = detail::simd_matrix<float1, 3>;

/// @brief
using float4x4 = detail::simd_matrix<float1, 4>;

/// @brief
using int2 = detail::simd_vector<int1, 2>;

/// @brief
using int3 = detail::simd_vector<int1, 3>;

/// @brief
using int4 = detail::simd_vector<int1, 4>;

/// @brief
using int3x3 = detail::simd_matrix<int1, 3>;

/// @brief
using int4x4 = detail::simd_matrix<int1, 4>;

/// @brief
using uint2 = detail::simd_vector<uint1, 2>;

/// @brief
using uint3 = detail::simd_vector<uint1, 3>;

/// @brief
using uint4 = detail::simd_vector<uint1, 4>;

/// @brief
using uint3x3 = detail::simd_matrix<uint1, 3>;

/// @brief
using uint4x4 = detail::simd_matrix<uint1, 4>;

/// @brief
using bool2 = detail::simd_vector<bool1, 2>;

/// @brief
using bool3 = detail::simd_vector<bool1, 3>;

/// @brief
using bool4 = detail::simd_vector<bool1, 4>;

/// @brief
using bool3x3 = detail::simd_matrix<bool1, 3>;

/// @brief
using bool4x4 = detail::simd_matrix<bool1, 4>;

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr inline static value_t zero = static_cast<value_t>(0.0);

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr bool1 is_zero(const value_t value);

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr inline static value_t infinity = std::numeric_limits<value_t>::max();

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr bool1 is_infinite(const value_t value);

/// @brief
/// @tparam value_t
template <typename value_t>
constexpr bool1 is_finite(const value_t value);

/// @brief
/// @tparam value_t
template <typename value_t>
std::string to_string(const value_t& value);
}

#include <bungeegum/glue/simd.inl>