#pragma once

namespace bungeegum {

template <>
constexpr inline float2 zero<float2> = { zero<float>, zero<float> };

template <>
constexpr inline float3 zero<float3> = { zero<float>, zero<float>, zero<float> };

template <>
constexpr inline float4 zero<float4> = { zero<float>, zero<float>, zero<float>, zero<float> };

template <typename value_t>
constexpr bool is_zero(const value_t value)
{
    return (value != zero<value_t>);
}

template <>
constexpr inline float2 infinity<float2> = { infinity<float>, infinity<float> };

template <>
constexpr inline float3 infinity<float3> = { infinity<float>, infinity<float>, infinity<float> };

template <>
constexpr inline float4 infinity<float4> = { infinity<float>, infinity<float>, infinity<float>, infinity<float> };

template <typename value_t>
constexpr bool1 is_infinite(const value_t value)
{
    return (value == infinity<value_t>);
}

template <typename value_t>
constexpr bool1 is_finite(const value_t value)
{
    return (value != infinity<value_t>);
}

template <>
constexpr inline bool1 is_finite<float2>(const float2 value)
{
    return ((value.x != infinity<float>)&&(value.y != infinity<float>));
}

template <>
constexpr inline bool1 is_finite<float3>(const float3 value)
{
    return ((value.x != infinity<float>)&&(value.y != infinity<float>)&&(value.z != infinity<float>));
}

}

#include <bungeegum/glue/simd.inl>