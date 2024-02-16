#pragma once

#include <bungeegum/core/widget.fwd>

namespace bungeegum {

/// @brief
enum struct interaction_type {
    window_resized,
    mouse_moved,
    mouse_down,
    mouse_up,
    mouse_pressed,
    mouse_wheel,
    key_down,
    key_up,
    key_pressed,
};

/// @brief
enum struct mouse_button {
    left,
    middle,
    right,
    additional_0,
    additional_1,
};

/// @brief
struct window_resized_interaction {

    /// @brief
    /// @return
    [[nodiscard]] bool is_fullscreen() const;

    /// @brief
    /// @return
    [[nodiscard]] float2 new_size() const;

private:
    detail::window_resized_event _event;
    friend struct interact_command;
    // friend struct detail::process_manager;
};

/// @brief
struct mouse_moved_interaction {

    /// @brief
    /// @return
    [[nodiscard]] float2 absolute_position() const;

    /// @brief
    /// @return
    [[nodiscard]] float2 relative_position() const;

private:
    detail::mouse_moved_event _event;
    friend struct interact_command;
    // friend struct detail::process_manager;
};

/// @brief
struct mouse_down_interaction {

    /// @brief
    /// @return
    [[nodiscard]] mouse_button button() const;

private:
    detail::mouse_down_event _event;
    friend struct interact_command;
    // friend struct detail::process_manager;
};

/// @brief
struct mouse_up_interaction {

    /// @brief
    /// @return
    [[nodiscard]] mouse_button button() const;

private:
    detail::mouse_up_event _event;
    friend struct interact_command;
    // friend struct detail::process_manager;
};

/// @brief
struct mouse_pressed_interaction {

    /// @brief
    /// @return
    [[nodiscard]] mouse_button button() const;

private:
    detail::mouse_pressed_event _event;
    friend struct interact_command;
    // friend struct detail::process_manager;
};

/// @brief
/// @details
struct interact_command {
    interact_command() = delete;
    interact_command(const interact_command& other) = delete;
    interact_command& operator=(const interact_command& other) = delete;
    interact_command(interact_command&& other) = delete;
    interact_command& operator=(interact_command&& other) = delete;

    /// @brief
    /// @return
    [[nodiscard]] interaction_type type() const;

    /// @brief
    /// @return
    [[nodiscard]] std::optional<window_resized_interaction> window_resized() const;

    /// @brief
    /// @return
    [[nodiscard]] std::optional<mouse_moved_interaction> mouse_moved() const;

    /// @brief
    /// @return
    [[nodiscard]] std::optional<mouse_down_interaction> mouse_down() const;

    /// @brief
    /// @return
    [[nodiscard]] std::optional<mouse_up_interaction> mouse_up() const;

    /// @brief
    /// @return
    [[nodiscard]] std::optional<mouse_pressed_interaction> mouse_pressed() const;

    /// @brief
    /// @param child_id
    void interact_child(const widget_id child_id);

    /// @brief
    void interact_children();

private:
    friend struct detail::interact_command_access;
    interact_command(const detail::interact_command_data& data);
    detail::interact_command_data _data;
};

/// @brief
/// @details
struct resolve_command {
    resolve_command() = delete;
    resolve_command(const resolve_command& other) = delete;
    resolve_command& operator=(const resolve_command& other) = delete;
    resolve_command(resolve_command&& other) = delete;
    resolve_command& operator=(resolve_command&& other) = delete;

    /// @brief
    [[nodiscard]] float2 get_min_size() const;

    /// @brief
    [[nodiscard]] float2 get_max_size() const;

    /// @brief
    /// @param size
    void resize(const float2 size);

    /// @brief
    /// @param child_widget
    /// @param constraint
    float2 resolve_child(const widget_id child_widget, const float2 min_size, const float2 max_size) const;

    /// @brief
    /// @param min_size
    /// @param max_size
    /// @return
    std::vector<float2> resolve_children(const float2 min_size, const float2 max_size);

    /// @brief
    /// @param child_widget
    /// @param position
    void position_child(const widget_id child_widget, const float2 position);

    /// @brief
    /// @param position
    void position_children(const float2 position);

    /// @brief
    /// @param child_id
    /// @param transform
    void rotate_child(const widget_id child_id, const float degrees = 180.f);

    /// @brief
    /// @param child_id
    /// @param transform
    void rotate_child(const widget_id child_id, const float3 rotation);

    /// @brief
    /// @param transform
    void rotate_children(const float3 rotation);

    /// @brief
    /// @param transform
    void rotate_children(const float degrees = 180.f);

private:
    friend struct detail::resolve_command_access;
    resolve_command(const detail::resolve_command_data& data);
    detail::resolve_command_data _data;
};

/// @brief
/// @details
struct draw_command {
    draw_command() = delete;
    draw_command(const draw_command& other) = delete;
    draw_command& operator=(const draw_command& other) = delete;
    draw_command(draw_command&& other) = delete;
    draw_command& operator=(draw_command&& other) = delete;

    /// @brief
    [[nodiscard]] float2 get_size() const;

    // /// @brief Sets the clipping rectangle for all the commands issued to this widget and all the
    // /// commands issued by its children.
    // /// @param first_point is the minimum point for this clipping rectangle
    // /// @param second_point is the maximum point for this clipping rectangle
    // void clip_rect(const float2 first_point, const float2 second_point); //

    void clip_mesh(const std::vector<float2>& positions, const std::vector<std::size_t>& indices);

    void orthographic_projection();

    void perspective_projection(const float fov);

    /// @brief
    /// @param child_id
    void draw_child(const widget_id child_id);

    /// @brief
    void draw_children();

    /// @brief
    void use_default_shader();

    /// @brief
    /// @param shader
    void use_custom_shader(const shader_ref& shader);

    /// @brief
    /// @param texture
    /// @param position
    /// @param size
    void draw_texture(const texture_ref& texture, const float2 position, const float2 size);

    // void draw_text(const std::string text, const font& text_font); // + FONT ?

    /// @brief
    /// @param first_point
    /// @param second_point
    /// @param color
    /// @param thickness
    void draw_line(const float2 first_point, const float2 second_point, const float4 color, const float thickness = 1.f);

    /// @brief
    /// @param min_point
    /// @param max_point
    /// @param color
    /// @param rounding_strength
    /// @param thickness
    void draw_rect(const float2 min_point, const float2 max_point, const float4 color, const float rounding_strength = 0.f, const float thickness = 1.f);

    /// @brief
    /// @param min_point
    /// @param max_point
    /// @param color
    /// @param rounding
    void draw_rect_filled(const float2 min_point, const float2 max_point, const float4 color, const float rounding = 0.f);

    /// @brief
    /// @param min_corner
    /// @param max_corner
    /// @param color_top_left
    /// @param color_top_right
    /// @param color_bottom_left
    /// @param color_bottom_right
    void draw_rect_filled_multi_color(const float2 min_corner, const float2 max_corner, const float4 color_top_left, const float4 color_top_right, const float4 color_bottom_left, const float4 color_bottom_right);

    /// @brief
    /// @param top_left_corner
    /// @param top_right_corner
    /// @param bottom_left_corner
    /// @param bottom_right_corner
    /// @param color
    /// @param thickness
    void draw_quad(const float2 top_left_corner, const float2 top_right_corner, const float2 bottom_left_corner, const float2 bottom_right_corner, const float4 color, const float thickness = 1.f);

    /// @brief
    /// @param top_left_corner
    /// @param top_right_corner
    /// @param bottom_left_corner
    /// @param bottom_right_corner
    /// @param color
    void draw_quad_filled(const float2 top_left_corner, const float2 top_right_corner, const float2 bottom_left_corner, const float2 bottom_right_corner, const float4 color);

    /// @brief
    /// @param first_corner
    /// @param second_corner
    /// @param third_corner
    /// @param color
    /// @param thickness
    void draw_triangle(const float2 first_corner, const float2 second_corner, const float2 third_corner, const float4 color, const float thickness = 1.f);

    /// @brief
    /// @param first_corner
    /// @param second_corner
    /// @param third_corner
    /// @param color
    void draw_triangle_filled(const float2 first_corner, const float2 second_corner, const float2 third_corner, const float4 color);

    // draw bezier and bspline from points direct (sans curve)

private:
    friend struct detail::draw_command_access;
    draw_command(const detail::draw_command_data& data);
    detail::draw_command_data _data;
};

/// @brief Opaque untyped
/// @details Instances of this type can be copied (shallow copy) and moved.
struct widget_id {
    widget_id() = delete;
    widget_id(const widget_id& other) = default;
    widget_id& operator=(const widget_id& other) = default;
    widget_id(widget_id&& other) = default;
    widget_id& operator=(widget_id&& other) = default;

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_t* widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_t& widget);

    /// @brief
    /// @tparam widget_t
    template <typename widget_t>
    widget_id(widget_ref<widget_t>& widget);

private:
    friend struct detail::widget_id_access;
    widget_id(const detail::widget_id_data& data);
    detail::widget_id_data _data;
};

/// @brief
/// @details Instances of this type can be copied (shallow copy) and moved.
/// @tparam widget_t
template <typename widget_t>
struct widget_ref {
    widget_ref() = delete;
    widget_ref(const widget_ref& other) = default;
    widget_ref& operator=(const widget_ref& other) = default;
    widget_ref(widget_ref&& other) = default;
    widget_ref& operator=(widget_ref&& other) = default;

    /// @brief
    widget_t& get() const;

    /// @brief
    widget_t* operator->() const;

private:
    friend struct detail::widget_ref_access<widget_t>;
    widget_ref(const detail::widget_ref_data<widget_t>& data);
    detail::widget_ref_data<widget_t> _data;
};

/// @brief
/// @param parent_id
/// @param child_id
void adopt(const widget_id parent_id, const widget_id child_id);

/// @brief
/// @param parent_id
/// @param child_id
void abandon(const widget_id parent_id, const widget_id child_id);

/// @brief Destroys a widget created with the make_reference function. References to it will no longer
/// be valid.
/// @param id
void destroy(const widget_id id);

/// @brief
/// @param id
/// @return
[[nodiscard]] std::vector<widget_id> get_children(const widget_id id);

/// @brief
/// @param id
/// @return
[[nodiscard]] widget_id get_parent(const widget_id id);

/// @brief
/// @tparam widget_t
/// @param id
/// @return
template <typename widget_t>
[[nodiscard]] widget_ref<widget_t> get_ref(const widget_id id);

/// @brief
/// @param widget
/// @return
[[nodiscard]] bool has_parent(const widget_id widget);

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
[[nodiscard]] bool has_type(const widget_id widget);

/// @brief
/// @tparam widget_t
template <typename widget_t>
[[nodiscard]] widget_t& make();

/// @brief Creates a new widget managed by bungeegum and returns a reference to it. This reference
/// will be valid until it is destroyed by the user.
/// @tparam widget_t Custom type for the widget. Must be at least copy or move constructible.
/// @return Returns a reference to the new widget
template <typename widget_t>
[[nodiscard]] widget_ref<widget_t> make_reference();

/// @brief
/// @param widget
void must_resolve(const widget_id id);

/// @brief
/// @param widget
void must_draw(const widget_id id);

// traverse
}

#include <bungeegum/core/widget.inl>
