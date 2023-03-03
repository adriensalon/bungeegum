//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <bungeegum/core/widget.hpp>

namespace bungeegum {
namespace detail {

    untyped_widget_data::untyped_widget_data() { }

    untyped_widget_data::untyped_widget_data(untyped_widget_data&& other)
    {
        *this = std::move(other);
    }

    untyped_widget_data& untyped_widget_data::operator=(untyped_widget_data&& other)
    {
        // is_built = other.is_built;
        kind = std::move(other.kind);
        parent = std::move(other.parent);
        children = std::move(other.children);
        detached_events_removers = std::move(other.detached_events_removers);
        widget_resolver = std::move(other.widget_resolver);
        widget_resolver_data = std::move(other.widget_resolver_data);
        widget_drawer = std::move(other.widget_drawer);
        widget_drawer_data = std::move(other.widget_drawer_data);
        return *this;
    }

    untyped_widget_data::~untyped_widget_data()
    {
        for (auto& _detached_event_remover : detached_events_removers)
            _detached_event_remover.second();
    }

    // widgets_registry::widgets_registry()
    // {
    // }

    // widgets_registry::widgets_registry(widgets_registry&& other)
    // {
    //     *this = std::move(other);
    // }

    // widgets_registry& widgets_registry::operator=(widgets_registry&& other)
    // {
    //     widgets = std::move(other.widgets);
    //     root = std::move(other.root);
    //     return *this;
    // }

    void widgets_registry::clear_resolve()
    {
        must_resolve_heads.clear();
    }

    void widgets_registry::clear_draw()
    {
        must_draw_heads.clear();
    }

    bool widgets_registry::is_parent(const untyped_widget_data& parent, const untyped_widget_data& child)
    {
        bool _found = false;
        // const untyped_widget_data* _ancestor_widget_data_ptr = nullptr;
        std::function<void(const untyped_widget_data&, const untyped_widget_data&)> _iterate = [&](const untyped_widget_data& _parent, const untyped_widget_data& _child) {
            // _ancestor_widget_data_ptr = &(_child);
            if (_child.parent.has_value()) {
                if (&(_child.parent.value().get()) == &_parent)
                    _found = true;
                else
                    _iterate(_parent, _child.parent.value().get());
            }
        };
        _iterate(parent, child);
        return _found;
    }

    void widgets_registry::iterate_must_resolve(const std::function<void(untyped_widget_data&, const bool)>& iterate_callback)
    {
        for (auto& _must_resolve : must_resolve_heads)
            iterate_callback(_must_resolve.first.get(), _must_resolve.second);
    }

    void widgets_registry::iterate_must_draw(const std::function<void(untyped_widget_data&, const bool)>& iterate_callback)
    {
        for (std::pair<std::reference_wrapper<bungeegum::detail::untyped_widget_data>, bool>& _must_draw : must_draw_heads)
            iterate_callback(_must_draw.first.get(), _must_draw.second);
    }

    bool widgets_registry::is_must_draw_empty() const
    {
        return must_draw_heads.empty();
    }

    void widgets_registry::iterate(const std::function<void(untyped_widget_data&)>& iterate_callback)
    {
        std::function<void(untyped_widget_data&)> _iterate = [&](untyped_widget_data& _widget_data) {
            iterate_callback(_widget_data);
            for (auto& _child : _widget_data.children)
                _iterate(_child);
        };
        if (root.has_value())
            _iterate(root.value().get());
        else
            std::cout << "ROOT IS NULL \n";
    }
}
}