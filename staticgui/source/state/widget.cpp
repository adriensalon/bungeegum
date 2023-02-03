//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/state/widget.hpp>

namespace staticgui {
namespace detail {

    widget_registry::widget_registry()
    {
    }

    widget_registry::widget_registry(widget_registry&& other)
    {
        *this = std::move(other);
    }

    widget_registry& widget_registry::operator=(widget_registry&& other)
    {
        _registry = std::move(other._registry);
        _roots = std::move(other._roots);
        return *this;
    }

    void widget_registry::clear_resolve()
    {
        _must_resolve_heads.clear();
    }

    void widget_registry::clear_draw()
    {
        _must_draw_heads.clear();
    }

    bool widget_registry::is_parent(const widget_data& parent, const widget_data& child)
    {
        bool _found = false;
        const widget_data* _ancestor_widget_data_ptr = nullptr;
        std::function<void(const widget_data&, const widget_data&)> _iterate = [&](const widget_data& _parent, const widget_data& _child) {
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

    void widget_registry::iterate_must_resolve(const std::function<void(widget_data&, const bool)>& iterate_callback)
    {
        for (auto& _must_resolve : _must_resolve_heads)
            iterate_callback(_must_resolve.first.get(), _must_resolve.second);
    }

    void widget_registry::iterate_must_draw(const std::function<void(widget_data&, const bool)>& iterate_callback)
    {
        // std::cout << "ITERATE MUST DRAW" << std::endl;
        for (std::pair<std::reference_wrapper<staticgui::detail::widget_data>, bool>& _must_draw : _must_draw_heads)
            iterate_callback(_must_draw.first.get(), _must_draw.second);
    }

    void widget_registry::iterate_datas(const std::function<void(widget_data&)>& iterate_callback)
    {
        std::function<void(widget_data&)> _iterate = [&](widget_data& _widget_data) {
            iterate_callback(_widget_data);
            for (auto& _child : _widget_data.children)
                _iterate(_child);
        };
        for (auto& _root : _roots)
            _iterate(_root);
    }

    unsigned int widget_registry::get_depth(widget_data& data)
    {
        unsigned int _depth = -1;
        widget_data* _ancestor_widget_data_ptr = nullptr;
        std::function<void(widget_data&)> _iterate = [&](widget_data& _widget_data) {
            _depth++;
            _ancestor_widget_data_ptr = &(_widget_data);
            if (_widget_data.parent.has_value()) {
                _iterate(_widget_data.parent.value().get());
            }
        };
        _iterate(data);
        for (auto& _root : _roots)
            if (&(_root.get()) == _ancestor_widget_data_ptr)
                return _depth;
        return _depth + 11;
    }
}
}