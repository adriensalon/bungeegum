#pragma once

#include <bungeegum/core/draw.fwd>

namespace bungeegum {

struct clipping_mesh {
    std::vector<float2> positions;
    std::vector<uint1> indices;
};

/// @brief
/// @details
struct draw_command {

    /// @brief Stops propagation
    void block();

    /// @brief
    [[nodiscard]] float2 resolved_position() const;

    /// @brief
    [[nodiscard]] float2 resolved_size() const;

    // /// @brief Sets the clipping rectangle for all the commands issued to this widget and all the
    // /// commands issued by its children.
    // /// @param first_point is the minimum point for this clipping rectangle
    // /// @param second_point is the maximum point for this clipping rectangle
    // void clip_rect(const float2 first_point, const float2 second_point); //

    void clip_mesh(const std::vector<float2>& positions, const std::vector<uint1>& indices);

    void orthographic_projection();

    void perspective_projection(const float1 fov);

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
    detail::draw_command_data _data;
    friend struct detail::process_manager;
#if BUNGEEGUM_USE_OVERLAY
    friend struct detail::widget_inspector;
#endif
};

/// @brief
/// @param widget
/// @param draw_callback
void on_draw(const runtime_widget& widget, const std::function<void(draw_command&)>& draw_callback);

/// @brief
void must_draw();

/// @brief
/// @param widget
void must_draw(const runtime_widget& widget);
}
