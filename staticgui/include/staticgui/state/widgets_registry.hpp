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
#include <staticgui/state/layout_state.hpp>

namespace staticgui {
namespace detail {

    struct widget_disabled {
    };

    struct widget_data {
        bool is_built = false;
        std::unique_ptr<std::type_index> kind = nullptr;
        std::function<void(layout_state&)> paint_callback = nullptr;
        std::optional<std::reference_wrapper<widget_data>> parent = std::nullopt;
        std::vector<std::reference_wrapper<widget_data>> children = {};
    };

    struct widgets_registry {
        widgets_registry();
        widgets_registry(const widgets_registry& other) = delete;
        widgets_registry& operator=(const widgets_registry& other) = delete;
        widgets_registry(widgets_registry&& other);
        widgets_registry& operator=(widgets_registry&& other);

        template <typename widget_t, typename... widget_args_t>
        widget_t& make(widget_args_t&&... widget_args);

        template <typename widget_t, typename... children_widgets_t>
        void build(widget_t* widget, std::function<void(layout_state&)> paint_callback, children_widgets_t&... children);

        template <typename... children_widgets_t>
        void build_roots(children_widgets_t&... children);

        void paint_all();

        template <typename widget_t>
        widget_data& get_data(widget_t& widget);

        template <typename widget_t>
        widget_t& get_widget(widget_data& data);

        template <typename widget_t, typename parent_widget_t> // invert
        std::optional<parent_widget_t&> get_parent_widget(widget_t& widget);

        unsigned int get_depth(widget_data& data);

        void iterate_datas(const std::function<void(widget_data&)>& iterate_callback);

        template <typename widget_t>
        void iterate_widgets(const std::function<void(widget_t&)>& iterate_callback);

    private:
        glue::registry _registry;
        std::vector<std::reference_wrapper<widget_data>> _roots;
    };

}
}

#include <staticgui/state/widgets_registry.inl>