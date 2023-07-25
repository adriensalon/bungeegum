#pragma once

#include <bungeegum/core/interact.fwd>

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
    friend struct detail::process_manager;
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
    friend struct detail::process_manager;
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
struct mouse_down_interaction {

    /// @brief
    /// @return
    [[nodiscard]] mouse_button button() const;

private:
    detail::mouse_down_event _event;
    friend struct interact_command;
    friend struct detail::process_manager;
};

/// @brief
struct mouse_up_interaction {

    /// @brief
    /// @return
    [[nodiscard]] mouse_button button() const;

private:
    detail::mouse_up_event _event;
    friend struct interact_command;
    friend struct detail::process_manager;
};

/// @brief
struct mouse_pressed_interaction {

    /// @brief
    /// @return
    [[nodiscard]] mouse_button button() const;

private:
    detail::mouse_pressed_event _event;
    friend struct interact_command;
    friend struct detail::process_manager;
};

/// @brief
/// @details
struct interact_command {

    /// @brief Stops propagation
    void block();

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

private:
    detail::interact_command_data _data;
    friend struct detail::process_manager;
#if BUNGEEGUM_USE_OVERLAY
    friend struct detail::widget_inspector;
#endif
};

/// @brief
/// @param widget
/// @param interact_callback
void on_interact(const runtime_widget& widget, const std::function<void(interact_command&)>& interact_callback);

}
