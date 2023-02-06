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

#include <staticgui/glue/constexpr.hpp>
#include <staticgui/glue/registry.hpp>
#include <staticgui/state/animation.hpp>
#include <staticgui/state/event.hpp>
#include <staticgui/state/layout.hpp>
#include <staticgui/state/rendering.hpp>

namespace staticgui {
namespace detail {

    struct widget_disabled {
    };

    struct widget_data {
        widget_data();
        widget_data(const widget_data& other) = delete;
        widget_data& operator=(const widget_data&& other) = delete;
        widget_data(widget_data&& other);
        widget_data& operator=(widget_data&& other);
        ~widget_data();

        bool is_built = false;
        std::unique_ptr<std::type_index> kind = nullptr;
        resolve_function resolver = nullptr;
        draw_function drawer = nullptr;
        std::optional<command_data> command = std::nullopt;
        std::optional<std::reference_wrapper<widget_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<widget_data>> children = {};
        std::unordered_map<glue::entity, std::function<void()>> detached_events_removers;
    };

    struct widget_registry {
        widget_registry();
        widget_registry(const widget_registry& other) = delete;
        widget_registry& operator=(const widget_registry& other) = delete;
        widget_registry(widget_registry&& other);
        widget_registry& operator=(widget_registry&& other);

        template <typename widget_t, typename... widget_args_t>
        widget_t& make(widget_args_t&&... widget_args);

        template <typename widget_t, typename... children_widgets_t>
        void declare(widget_t* widget, children_widgets_t&... children);

        template <typename widget_t>
        void on_resolve(widget_t* widget, const resolve_function& resolver);

        template <typename widget_t>
        void on_draw(widget_t* widget, const draw_function& drawer);

        template <typename... children_widgets_t>
        void declare_root(children_widgets_t&... children);

        template <typename widget_t>
        void must_resolve(widget_t& widget, const bool must_resolve_children = true);

        template <typename widget_t>
        void must_draw(widget_t& widget, const bool must_draw_children = true);

        void clear_resolve();

        void clear_draw();

        bool is_parent(const widget_data& parent, const widget_data& child);

        template <typename widget_t>
        widget_data& get_data(widget_t& widget);

        template <typename widget_t>
        widget_t& get_widget(widget_data& data);

        void iterate_must_resolve(const std::function<void(widget_data&, const bool)>& iterate_callback);

        void iterate_must_draw(const std::function<void(widget_data&, const bool)>& iterate_callback);

        bool is_must_draw_empty() const;

        void iterate_datas(const std::function<void(widget_data&)>& iterate_callback);

        template <typename widget_t>
        void iterate_widgets(const std::function<void(widget_t&)>& iterate_callback);

        unsigned int get_depth(widget_data& data);

        template <typename... values_t, typename widget_t>
        void attach_event(event_impl<values_t...>& event, widget_t& widget);

    private:
        glue::registry _registry;
        std::vector<std::reference_wrapper<widget_data>> _roots;
        std::vector<std::pair<std::reference_wrapper<widget_data>, bool>> _must_resolve_heads;
        std::vector<std::pair<std::reference_wrapper<widget_data>, bool>> _must_draw_heads;
    };

}
}

#include <staticgui/state/widget.inl>