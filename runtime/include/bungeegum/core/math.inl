#pragma once

#include <iomanip>
#include <sstream>
#include <type_traits>

namespace bungeegum {

namespace detail {
    namespace traits {

        // type has addition
        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_outside_add_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_add_v<value_t, std::void_t<decltype(operator+(std::declval<value_t>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_inside_add_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_inside_add_v<value_t, std::void_t<decltype(std::declval<value_t>().operator+(std::declval<value_t>()))>> = true;

        template <typename value_t>
        constexpr bool has_add_v = (has_outside_add_v<value_t> || has_inside_add_v<value_t>);

        // type has float multiplication
        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_outside_float_right_multiply_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_float_right_multiply_v<value_t, std::void_t<decltype(operator*(std::declval<value_t>(), std::declval<float>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_outside_float_left_multiply_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_outside_float_left_multiply_v<value_t, std::void_t<decltype(operator*(std::declval<float>(), std::declval<value_t>()))>> = true;

        template <typename value_t, typename = std::void_t<>>
        constexpr bool has_inside_float_multiply_v = std::is_arithmetic_v<value_t>;

        template <typename value_t>
        constexpr bool has_inside_float_multiply_v<value_t, std::void_t<decltype(std::declval<value_t>().operator*(std::declval<float>()))>> = true;

        template <typename value_t>
        constexpr bool has_float_multiply_v = (has_outside_float_left_multiply_v<value_t> || has_outside_float_right_multiply_v<value_t> || has_inside_float_multiply_v<value_t>);

        // type has both
        template <typename value_t>
        constexpr bool is_lerpable_v = (has_float_multiply_v<value_t> && has_add_v<value_t>);

        template <typename value_t>
        using lerpable_t = typename std::enable_if_t<is_lerpable_v<value_t>, value_t>;
    }
}

template <>
constexpr inline float2 zero<float2> = { zero<float>, zero<float> };

template <>
constexpr inline float3 zero<float3> = { zero<float>, zero<float>, zero<float> };

template <>
constexpr inline float4 zero<float4> = { zero<float>, zero<float>, zero<float>, zero<float> };

template <typename value_t>
constexpr bool is_zero(const value_t value)
{
    return value != zero<value_t>;
}

template <>
constexpr inline float2 infinity<float2> = { infinity<float>, infinity<float> };

template <>
constexpr inline float3 infinity<float3> = { infinity<float>, infinity<float>, infinity<float> };

template <>
constexpr inline float4 infinity<float4> = { infinity<float>, infinity<float>, infinity<float>, infinity<float> };

template <typename value_t>
constexpr bool is_infinite(const value_t value)
{
    return value == infinity<value_t>;
}

template <typename value_t>
constexpr bool is_finite(const value_t value)
{
    return value != infinity<value_t>;
}

template <>
constexpr inline bool is_finite<float2>(const float2 value)
{
    return value.x != infinity<float> && value.y != infinity<float>;
}

template <>
constexpr inline bool is_finite<float3>(const float3 value)
{
    return value.x != infinity<float> && value.y != infinity<float> && value.z != infinity<float>;
}

template <>
constexpr inline float3x3 identity<float3x3> = {
    1.f, zero<float>, zero<float>, 
    zero<float>, 1.f, zero<float>, 
    zero<float>, zero<float>, 1.f
};

template <>
constexpr inline float4x4 identity<float4x4> = {
    1.f, zero<float>, zero<float>, zero<float>,
    zero<float>, 1.f, zero<float>, zero<float>,
    zero<float>, zero<float>, 1.f, zero<float>,
    zero<float>, zero<float>, zero<float>, 1.f
};

template <typename value_t>
inline std::string to_string(const value_t& value)
{
    std::stringstream _stream;
    _stream << std::fixed << std::setprecision(1) << value;
    return _stream.str();
}

template <>
inline std::string to_string<float2>(const float2& value)
{
    std::stringstream _stream;
    _stream << std::fixed << std::setprecision(1) << value.x << ", " << value.y;
    return _stream.str();
}

template <>
inline std::string to_string<float3>(const float3& value)
{
    std::stringstream _stream;
    _stream << std::fixed << std::setprecision(1) << value.x << ", " << value.y << ", " << value.z;
    return _stream.str();
}

template <>
inline std::string to_string<float4>(const float4& value)
{
    std::stringstream _stream;
    _stream << std::fixed << std::setprecision(1) << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
    return _stream.str();
}

namespace math {

    template <typename value_t>
    value_t lerp(const value_t& min_value, const value_t& max_value, const float t)
    {
        static_assert(bungeegum::detail::traits::has_add_v<value_t>, "This type is not lerpable because it is not arithmetic and doesn't "
                                                                     "implement the operator+(const value_t&) method.");
        static_assert(bungeegum::detail::traits::has_float_multiply_v<value_t>, "This type is not lerpable because it is not arithmetic and doesn't "
                                                                                "implement the operator*(const float&) method.");
        return min_value * (1.f - t) + max_value * t;
    }

}
}