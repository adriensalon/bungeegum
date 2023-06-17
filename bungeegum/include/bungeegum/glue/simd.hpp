#pragma once

#include <limits>

#include <glm/glm.hpp>

namespace bungeegum {

using float1 = float;
using float2 = glm::vec2;
using float3 = glm::vec3;
using float4 = glm::vec4;
using float3x3 = glm::mat3;
using float4x4 = glm::mat4;

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
