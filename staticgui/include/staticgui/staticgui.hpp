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

#include <functional>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

/// @brief
namespace staticgui {

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
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief
namespace context {

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

        template <typename root_widget_t>
        static void append(painter& p, const root_widget_t& root) { }

    }
};

/// @brief
/// @tparam widget_t
/// @tparam child_widget_t
/// @param widget
/// @param child_widget
template <typename widget_t, typename child_widget_t>
void build(widget_t* widget, child_widget_t& child_widget);

/// @brief
/// @tparam parent_widget_t
/// @param  widget
/// @param  paint_callback
template <typename widget_t, typename... children_widgets_t>
void build(widget_t* widget, children_widgets_t&... children, std::function<void(context::advanced::painter&)> paint_callback);

#if defined(STATICGUI_DEBUG)

/// @brief
void print_build_tree();

#endif

}

/// @brief
/// @details
#define STATICGUI_WIDGET(widget_t)                                  \
    staticgui::internal::detail::runtime_widget_data internal_data; \
                                                                    \
    template <typename widget_t>                                    \
    friend staticgui::application& staticgui::launch(widget_t&);    \
                                                                    \
    template <typename widget_t, typename... widget_args_t>         \
    friend widget_t& staticgui::make(widget_args_t&&...);           \
                                                                    \
    template <typename widget_t, typename child_widget_t>           \
    friend void staticgui::build(widget_t*, child_widget_t&);       \
                                                                    \
    template <typename widget_t, typename... children_widgets_t>    \
    friend void staticgui::build(widget_t*, children_widgets_t&..., \
        std::function<void(staticgui::context::advanced::painter&)>);

#include "staticgui.inl"