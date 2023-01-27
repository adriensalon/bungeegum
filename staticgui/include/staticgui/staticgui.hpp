//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#define STATICGUI_DEBUG
#include <iostream>

#include <array>
#include <functional>
#include <future>
#include <string>
#include <variant>
#include <vector>

/// @brief
/// @details
#define STATICGUI_WIDGET(widget_t)

/// @brief
namespace staticgui {

struct color {
};

/// @brief
/// @tparam ...values_t
template <typename... values_t>
struct event {

    event() = delete;

    event(const event& other) = delete;

    event& operator=(const event& other) = delete;

    event(event&& other);

    event& operator=(event&& other);

    template <typename first_value_t = typename std::enable_if_t<sizeof...(values_t) == 1>, typename... other_values_t>
    struct unique_value {
        using type = first_value_t;
    };

    /// @brief
    /// @param trigger_callback
    /// @return
    event& on_trigger(std::function<void(const values_t&...)> trigger_callback);

    event& clear();

    /// @brief
    /// @param ...values
    void trigger(const values_t&... values);

    /// @brief
    /// @param future_value
    template <typename = typename std::enable_if_t<sizeof...(values_t) == 0>>
    void trigger(const std::future<void>& future_value);

    /// @brief
    /// @param future_value
    template <typename = typename std::enable_if_t<sizeof...(values_t) == 1>>
    void trigger(const std::future<typename unique_value<values_t...>::type>& future_value);

    /// @brief
    /// @param future_value
    template <typename = typename std::enable_if_t<sizeof...(values_t) >= 2>>
    void trigger(const std::future<std::tuple<values_t...>>& future_value);
};

/// @brief
/// @details
/// @tparam value_t
template <typename value_t>
value_t lerp(const value_t& min, const value_t& max, const float t);

template <typename value_t>
struct curve {

    curve(const value_t& min, const value_t& max);

    std::vector<std::pair<float, value_t>>& get_points();

    const std::vector<std::pair<float, value_t>>& get_points() const;

    value_t get_value(const float t);

private:
    // curve_impl _impl;
};

template <typename... values_t>
struct animation {

    animation(const curve<values_t>&... curves);

    animation(const animation& other) = delete;

    animation& operator=(const animation& other) = delete;

    animation(animation&& other);

    animation& operator=(animation&& other);

    animation& on_value_changed(std::function<void(const values_t&...)> value_changed_callback);

    template <typename... other_values_t>
    animation& start_after(const animation<other_values_t...>& previous);

    void start();

    void stop();

private:
};

template <typename value_t>
struct value {

    value(const animation<value_t>& animated_value);

    value(const value_t& static_value);

    void assign(value_t& target_value); // sets or registers animation callback ^^

private:
    bool _is_animated;
    std::variant<value_t, animation<value_t>> _value;
};

/// @brief
struct application {

    /// @brief
    /// @param title
    /// @return
    application& title(const std::string& window_title);

    /// @brief
    application& size(const unsigned int window_width, const unsigned int window_height);

    application& on_resized(std::function<void()> on_window_resized_callback);

    application& debug_stream(const std::ostream& stream = std::cerr);

    template <typename... root_widgets_t>
    application(std::pair<std::string, root_widgets_t&>... named_root_widgets) { }

    // move all navigation to context::replace !!! ^^

    template <typename root_widget_t>
    application& create_root(std::pair<std::string, root_widget_t&> named_root_widget);

    application& destroy_root(const std::string& named_root_widget);

    application& on_navigate_to(std::function<void(const float)> navigate_callback);

    application& on_navigate_to(const std::string& root_widget_name, std::function<void(const float)> navigate_callback);

    application& on_navigate_from(std::function<void(const float)> navigate_callback);

    application& on_navigate_from(const std::string& root_widget_name, std::function<void(const float)> navigate_callback);

    application& navigate(const std::string& root_widget_name, const curve<float>& transition_curve, const float duration);

    // input avec callbacks pareil

    application& on_key_pressed(std::function<void()> key_pressed_callback);
};

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
application& launch(widget_t& widget);

/// @brief
/// @tparam widget_t
/// @param widget
/// @return
template <typename widget_t>
application& attach(widget_t& widget);

/// @brief
namespace context {

    application& get_application();

    // navigation ?

    inline void erase() { }

    inline void erase_children() { }

    template <typename widget_t>
    void replace(const widget_t& widget) { }

    template <typename child_widget_t, typename widget_t>
    void replace(const child_widget_t& child_widget, const widget_t& widget) { }

    template <typename widget_t>
    void replace_children(const widget_t& widget) { }

    inline void maintain() { }

    template <typename child_widget_t>
    void maintain_child(const child_widget_t& child) { }

    inline void maintain_children() { }

    // rebuild

    namespace advanced {

        struct painter {
        };

        // cursor etc

        // im gui api here

    }
};

/// @brief
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief
/// @tparam widget_t
/// @tparam child_widget_t
/// @param widget
/// @param child_widget
template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget, const bool is_above_root_widgets = false);

/// @brief
/// @tparam parent_widget_t
/// @param  widget
/// @param  paint_callback
template <typename widget_t, typename... children_widgets_t>
void build(widget_t* widget, children_widgets_t&... children, std::function<void(context::advanced::painter&)> paint_callback, const bool is_above_root_widgets = false);

#if defined(STATICGUI_DEBUG)

/// @brief
void print_build_tree();

#endif
}

#include "staticgui.inl"

#include <staticgui/impl/animation.inl>
#include <staticgui/impl/build.inl>
#include <staticgui/impl/curve.inl>
#include <staticgui/impl/event.inl>
#include <staticgui/impl/lerp.inl>
#include <staticgui/impl/make.inl>
#include <staticgui/impl/value.inl>
#include <staticgui/impl/widgets.inl>