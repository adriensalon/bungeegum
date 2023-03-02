//   _
//  | |__   _   _  _ __    __ _   ___   ___   __ _  _   _  _ __ ___
//  | '_ \ | | | || '_ \  / _` | / _ \ / _ \ / _` || | | || '_ ` _ \ 
//  | |_) || |_| || | | || (_| ||  __/|  __/| (_| || |_| || | | | | |
//  |_.__/  \__,_||_| |_| \__, | \___| \___| \__, | \__,_||_| |_| |_|
//                        |___/              |___/

#pragma once

#include <chrono>
#include <optional>
#include <typeindex>

#include <bungeegum/advanced/draw.hpp>
#include <bungeegum/advanced/interact.hpp>
#include <bungeegum/advanced/resolve.hpp>
#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/event.hpp>
#include <bungeegum/glue/foreach.hpp>
#include <bungeegum/glue/registry.hpp>

namespace bungeegum {
namespace detail {

    struct untyped_widget_data {
        untyped_widget_data();
        untyped_widget_data(const untyped_widget_data& other) = delete;
        untyped_widget_data& operator=(const untyped_widget_data& other) = delete;
        untyped_widget_data(untyped_widget_data&& other);
        untyped_widget_data& operator=(untyped_widget_data&& other);
        ~untyped_widget_data();

        void* widget = nullptr; // on peut enlever ?, besoin pour get data -> typed
        std::unique_ptr<std::type_index> kind = nullptr;
        std::optional<std::reference_wrapper<untyped_widget_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<untyped_widget_data>> children;
        // sort -> index pour chaque untyped_widget_data -> entt::sort sur les
        // indices

        std::unordered_map<entity_t, std::function<void()>>
            detached_events_removers;

        std::function<float2(resolve_command_data&)> widget_resolver = nullptr;
        std::optional<resolve_command_data> widget_resolver_data = std::nullopt;

        std::function<void(const float2, draw_command_data&)> widget_drawer = nullptr;
        std::optional<draw_command_data> widget_drawer_data = std::nullopt;
    };

    // template <typename widget_t>
    // struct typed_widget_data {
    //     typed_widget_data(widget_t& widget)
    //         : widget_ref(widget)
    //     {
    //     }
    //     widget_t& widget_ref;
    // };

    // adopt
    // reny
    // replace

    struct widgets_registry {
        registry widgets;
        std::unordered_map<void*, entity_t> inline_widgets; // go pareil quen dessous
        std::unordered_map<void*, untyped_widget_data*> accessors; // go cpy ctor pour le untyped data qui move
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

    template <typename widget_t, typename... children_widgets_t>
    constexpr bool has_resolve_function_v = detail::is_detected_exact_v<float2, detail::resolve_function, widget_t, children_widgets_t...>;

    template <typename widget_t>
    using draw_function = decltype(std::declval<widget_t>().draw(std::declval<const float2&>(), std::declval<draw_command&>()));

    template <typename widget_t>
    constexpr bool has_draw_function_v = detail::is_detected_exact_v<void, detail::draw_function, widget_t>;
}

/// @brief
/// @tparam widget_t
/// @tparam ...widget_args_t
/// @param ...widget_args
/// @return
template <typename widget_t, typename... widget_args_t>
[[nodiscard]] widget_t& make(widget_args_t&&... widget_args);

/// @brief
/// @tparam widget_t
/// @tparam ...children_widgets_t
/// @param widget
/// @param ...children_widgets
template <typename widget_t, typename... children_widgets_t>
void adopt(widget_t* widget, children_widgets_t&... children_widgets);

/// @brief
/// @param widget
/// @param ...children_widgets
void abandon(widget_t* widget, children_widgets_t&... children_widgets);
}

#include <bungeegum/core/widget.inl>
