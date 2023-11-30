#include <bungeegum/core/log.hpp>
#include <bungeegum/core/runtime.hpp>

namespace bungeegum {
namespace detail {

    bool widgets_manager::contains(const std::uintptr_t raw_widget) const
    {
        return _updatables.find(raw_widget) != _updatables.end();
    }

    void widgets_manager::notify_erase(const std::uintptr_t raw_widget)
    {
        _updatables_to_erase.push_back(raw_widget);
    }

    widget_update_data& widgets_manager::operator[](const std::uintptr_t raw_widget)
    {
        return _updatables[raw_widget];
    }

    widget_update_data& widgets_manager::operator[](const runtime_widget& widget)
    {
        return _updatables[raw(widget)];
    }

    std::uintptr_t widgets_manager::raw(const runtime_widget& runtime_widget)
    {
        return runtime_widget._data.raw_widget;
    }

    std::uintptr_t& widgets_manager::root()
    {
        return _root;
    }

    std::uintptr_t widgets_manager::root() const
    {
        return _root;
    }

    widget_update_data& widgets_manager::root_update_data()
    {
        return _updatables[_root];
    }

    // const widget_update_data& widgets_manager::root_update_data() const
    // {
    //     return _updatables.insert_or_assign(_root);
    // }

    runtime_widget widgets_manager::create_runtime_widget(const std::uintptr_t raw_widget)
    {
        return runtime_widget(detail::runtime_widget_data { raw_widget });
    }

    runtime_widget widgets_manager::create_runtime_widget(widget_update_data& widget_data)
    {
        return runtime_widget(detail::runtime_widget_data { widget_data.raw_widget });
    }

    void widgets_manager::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback)
    {
        std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
            bool _continue = iterate_callback(_update_data);
            if (_continue) {
                for (auto& _child : _update_data.children) {
                    _iterate(_child.get());
                }
            }
        };
        _iterate(iterate_root);
    }

    void widgets_manager::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback,
        const std::function<void(widget_update_data&)>& tail_iterate_callback)
    {
        std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
            bool _continue = iterate_callback(_update_data);
            if (_continue) {
                for (auto& _child : _update_data.children) {
                    _iterate(_child.get());
                }
            }
            tail_iterate_callback(_update_data);
        };
        _iterate(iterate_root);
    }
}
}