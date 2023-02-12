//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <optional>
#include <typeindex>

#include <staticgui/context/animation.hpp>
#include <staticgui/context/draw.hpp>
#include <staticgui/context/event.hpp>
#include <staticgui/context/resolve.hpp>
#include <staticgui/glue/foreach.hpp>
#include <staticgui/glue/registry.hpp>

namespace staticgui {
namespace detail {

    struct widget_resolve_tag {
    };

    struct widget_draw_tag {
    };

    struct untyped_widget_data {
        untyped_widget_data();
        untyped_widget_data(const untyped_widget_data& other) = delete;
        untyped_widget_data& operator=(const untyped_widget_data& other) = delete;
        untyped_widget_data(untyped_widget_data&& other);
        untyped_widget_data& operator=(untyped_widget_data&& other);
        ~untyped_widget_data();

        std::unique_ptr<std::type_index> kind = nullptr;
        std::optional<std::reference_wrapper<untyped_widget_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<untyped_widget_data>> children;
        std::unordered_map<entity_t, std::function<void()>> detached_events_removers;
        std::function<float2(resolve_command_data&)> widget_resolver = nullptr;
        std::optional<resolve_command_data> widget_resolver_data = std::nullopt;
        std::function<void(const float2, draw_command_data&)> widget_drawer = nullptr;
        std::optional<draw_command_data> widget_drawer_data = std::nullopt;
    };

    struct widgets_registry {
        widgets_registry();
        widgets_registry(const widgets_registry& other) = delete;
        widgets_registry& operator=(const widgets_registry& other) = delete;
        widgets_registry(widgets_registry&& other);
        widgets_registry& operator=(widgets_registry&& other);

        registry widgets;
        std::optional<std::reference_wrapper<untyped_widget_data>> root;
        std::vector<std::pair<std::reference_wrapper<untyped_widget_data>, bool>> must_resolve_heads; // bool = must resolve children too
        std::vector<std::pair<std::reference_wrapper<untyped_widget_data>, bool>> must_draw_heads;

        void clear_resolve();
        void clear_draw();
        bool is_parent(const untyped_widget_data& parent, const untyped_widget_data& child);
        void iterate_must_resolve(const std::function<void(untyped_widget_data&, const bool)>& iterate_callback);
        void iterate_must_draw(const std::function<void(untyped_widget_data&, const bool)>& iterate_callback);
        bool is_must_draw_empty() const;
        void iterate(const std::function<void(untyped_widget_data&)>& iterate_callback);

        template <typename widget_t, typename... widget_args_t>
        widget_t& create(widget_args_t&&... widget_args);

        template <typename widget_t, typename... children_widgets_t>
        void build(widget_t* widget, children_widgets_t&... children);

        template <typename widget_t>
        void build_root(widget_t& widget);

        template <typename widget_t>
        void on_resolve(widget_t* widget, const std::function<float2(resolve_command_data&)>& resolver);

        template <typename widget_t>
        void on_draw(widget_t* widget, const std::function<void(const float2, draw_command_data&)>& drawer);

        template <typename widget_t>
        void must_resolve(widget_t& widget, const bool must_resolve_children = true);

        template <typename widget_t>
        void must_draw(widget_t& widget, const bool must_draw_children = true);

        template <typename widget_t>
        untyped_widget_data& get(widget_t& widget);

        template <typename widget_t>
        widget_t& get(untyped_widget_data& data);

        template <typename widget_t>
        void iterate(const std::function<void(widget_t&)>& iterate_callback);

        template <typename... values_t, typename widget_t>
        void detach(typed_event_data<values_t...>& event, widget_t& widget);

        template <typename value_t, typename widget_t>
        void detach(typed_animation_data<value_t>& animation, widget_t& widget);
    };

    inline widgets_registry widgets_context;

    template <typename widget_t, typename... children_widgets_t>
    using resolve_function = decltype(std::declval<widget_t>().resolve<children_widgets_t...>(std::declval<resolve_command&>(), std::declval<children_widgets_t&>()...));

    template <typename widget_t>
    using draw_function = decltype(std::declval<widget_t>().draw(std::declval<const float2&>(), std::declval<draw_command&>()));

}

/// @brief
template <typename widget_t, typename... children_widgets_t>
constexpr bool has_resolve_function_v = is_detected_exact_v<float2, detail::resolve_function, widget_t, children_widgets_t...>;

/// @brief
template <typename widget_t>
constexpr bool has_draw_function_v = is_detected_exact_v<void, detail::draw_function, widget_t>;

/// @brief
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& create(widget_args_t&&... widget_args);

/// @brief
/// @tparam widget_t
/// @tparam child_widget_t
/// @param widget
/// @param child_widget
template <typename widget_t, typename... children_widgets_t>
void build(widget_t* widget, children_widgets_t&... children_widgets);

/// @brief
/// @tparam widget_t
/// @param widget
template <typename widget_t>
void destroy(widget_t& widget);

// nav et input ici ?

}

#include <staticgui/context/widget.inl>