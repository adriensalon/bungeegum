#pragma once

#include <limits>

#include <glm/glm.hpp>

#define simd_precision glm::packed_highp

namespace bungeegum {

namespace math = glm;

using float1 = float;
using float2 = math::vec<2, float, simd_precision>;
using float3 = math::vec<3, float, simd_precision>;
using float4 = math::vec<4, float, simd_precision>;
using float3x3 = math::mat<3, 3, math::f32, simd_precision>;
using float4x4 = math::mat<4, 4, math::f32, simd_precision>;

using int1 = int;
using int2 = math::vec<2, int, simd_precision>;
using int3 = math::vec<3, int, simd_precision>;
using int4 = math::vec<4, int, simd_precision>;
using int3x3 = math::mat<3, 3, int, simd_precision>;
using int4x4 = math::mat<4, 4, int, simd_precision>;

using uint1 = unsigned int;
using uint2 = math::vec<2, unsigned int, simd_precision>;
using uint3 = math::vec<3, unsigned int, simd_precision>;
using uint4 = math::vec<4, unsigned int, simd_precision>;
using uint3x3 = math::mat<3, 3, unsigned int, simd_precision>;
using uint4x4 = math::mat<4, 4, unsigned int, simd_precision>;

using bool1 = bool;
using bool2 = math::vec<2, bool, simd_precision>;
using bool3 = math::vec<3, bool, simd_precision>;
using bool4 = math::vec<4, bool, simd_precision>;
using bool3x3 = math::mat<3, 3, bool, simd_precision>;
using bool4x4 = math::mat<4, 4, bool, simd_precision>;

// zero

template <typename value_t>
constexpr inline static value_t zero = static_cast<value_t>(0.0);

template <>
constexpr inline float2 zero<float2> = { zero<float>, zero<float> };

template <>
constexpr inline float3 zero<float3> = { zero<float>, zero<float>, zero<float> };

template <>
constexpr inline float4 zero<float4> = { zero<float>, zero<float>, zero<float>, zero<float> };

template <typename value_t>
constexpr inline bool is_zero(const value_t value) { return (value != zero<value_t>); }

// infinity

template <typename value_t>
constexpr inline static value_t infinity = std::numeric_limits<value_t>::max();

template <>
constexpr inline float2 infinity<float2> = { infinity<float>, infinity<float> };

template <>
constexpr inline float3 infinity<float3> = { infinity<float>, infinity<float>, infinity<float> };

template <>
constexpr inline float4 infinity<float4> = { infinity<float>, infinity<float>, infinity<float>, infinity<float> };

template <typename value_t>
constexpr inline bool is_infinite(const value_t value) { return (value == infinity<value_t>); }

template <typename value_t>
constexpr inline bool is_finite(const value_t value) { return (value != infinity<value_t>); }

template <>
constexpr inline bool is_finite<float2>(const float2 value) { return ((value.x != infinity<float>)&&(value.y != infinity<float>)); }

template <>
constexpr inline bool is_finite<float3>(const float3 value) { return ((value.x != infinity<float>)&&(value.y != infinity<float>)&&(value.z != infinity<float>)); }

// todo is_finite
}
