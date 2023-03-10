//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>
#include <vector>

#include <bungeegum/glue/simd.hpp>

struct ImDrawList;

namespace bungeegum {
namespace detail {

    struct draw_command_data {
        std::vector<std::function<void(ImDrawList*)>> commands;

        void draw(ImDrawList* imgui_drawlist);
        void draw();
    };

}

/// @brief
/// @details
struct draw_command {
    draw_command(detail::draw_command_data& data);
    draw_command(const draw_command& other);
    draw_command& operator=(const draw_command& other);
    draw_command(draw_command&& other);
    draw_command& operator=(draw_command&& other);

    /// @brief
    /// @param first_point
    /// @param second_point
    /// @param color
    /// @param thickness
    void draw_line(
        const float2 first_point, const float2 second_point,
        const float4 color,
        const float thickness = 1.f);

    /// @brief
    /// @param min_point
    /// @param max_point
    /// @param color
    /// @param rounding_strength
    /// @param thickness
    void draw_rect(
        const float2 min_point, const float2 max_point,
        const float4 color,
        const float rounding_strength = 0.f,
        const float thickness = 1.f);

    /// @brief
    /// @param min_point
    /// @param max_point
    /// @param color
    /// @param rounding
    void draw_rect_filled(
        const float2 min_point, const float2 max_point,
        const float4 color,
        const float rounding = 0.f);

    /// @brief
    /// @param min_corner
    /// @param max_corner
    /// @param color_top_left
    /// @param color_top_right
    /// @param color_bottom_left
    /// @param color_bottom_right
    void draw_rect_filled_multi_color(
        const float2 min_corner, const float2 max_corner,
        const float4 color_top_left, const float4 color_top_right,
        const float4 color_bottom_left, const float4 color_bottom_right);

    /// @brief
    /// @param top_left_corner
    /// @param top_right_corner
    /// @param bottom_left_corner
    /// @param bottom_right_corner
    /// @param color
    /// @param thickness
    void draw_quad(
        const float2 top_left_corner, const float2 top_right_corner,
        const float2 bottom_left_corner, const float2 bottom_right_corner,
        const float4 color,
        const float thickness = 1.f);

    /// @brief
    /// @param top_left_corner
    /// @param top_right_corner
    /// @param bottom_left_corner
    /// @param bottom_right_corner
    /// @param color
    void draw_quad_filled(
        const float2 top_left_corner, const float2 top_right_corner,
        const float2 bottom_left_corner, const float2 bottom_right_corner,
        const float4 color);

    /// @brief
    /// @param first_corner
    /// @param second_corner
    /// @param third_corner
    /// @param color
    /// @param thickness
    void draw_triangle(
        const float2 first_corner, const float2 second_corner, const float2 third_corner,
        const float4 color,
        const float thickness = 1.f);

    /// @brief
    /// @param first_corner
    /// @param second_corner
    /// @param third_corner
    /// @param color
    void draw_triangle_filled(
        const float2 first_corner, const float2 second_corner, const float2 third_corner,
        const float4 color);

    // draw bezier and bspline from points direct (sans curve)

private:
    detail::draw_command_data& _data;
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @param draw_callback
template <typename widget_t>
void on_draw(widget_t* widget, const std::function<void(draw_command&)>& draw_callback);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void must_draw(widget_t* widget);

}

#include <bungeegum/advanced/draw.inl>
