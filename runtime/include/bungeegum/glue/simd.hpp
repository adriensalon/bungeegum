#pragma once

#include <limits>

#include <glm/glm.hpp>

/// @brief
#if !defined(BUNGEEGUM_USE_SIMD_PRECISION)
#define BUNGEEGUM_USE_SIMD_PRECISION glm::packed_highp
#endif

namespace bungeegum {

/// @brief
namespace math = glm;

/// @brief
using float1 = float;

/// @brief
using float2 = math::vec<2, float, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using float3 = math::vec<3, float, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using float4 = math::vec<4, float, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using float3x3 = math::mat<3, 3, math::f32, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using float4x4 = math::mat<4, 4, math::f32, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using int1 = int;

/// @brief
using int2 = math::vec<2, int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using int3 = math::vec<3, int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using int4 = math::vec<4, int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using int3x3 = math::mat<3, 3, int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using int4x4 = math::mat<4, 4, int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using uint1 = unsigned int;

/// @brief
using uint2 = math::vec<2, unsigned int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using uint3 = math::vec<3, unsigned int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using uint4 = math::vec<4, unsigned int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using uint3x3 = math::mat<3, 3, unsigned int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using uint4x4 = math::mat<4, 4, unsigned int, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using bool1 = bool;

/// @brief
using bool2 = math::vec<2, bool, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using bool3 = math::vec<3, bool, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using bool4 = math::vec<4, bool, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using bool3x3 = math::mat<3, 3, bool, BUNGEEGUM_USE_SIMD_PRECISION>;

/// @brief
using bool4x4 = math::mat<4, 4, bool, BUNGEEGUM_USE_SIMD_PRECISION>;

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
}

#include <bungeegum/glue/simd.inl>