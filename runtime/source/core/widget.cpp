#include <imgui.h>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/widget.hpp>


namespace bungeegum {
namespace detail {
    namespace {
        void protect_userspace(std::vector<backtraced_exception>& container, const std::function<void()>& try_callback)
        {
            protect(try_callback, [&container](const std::string& _what) {
                // uncaught error in widget ... with message ... Please use the log_error() function to detect misconfiguration etc
                backtraced_exception _exception(_what, 0, 0);
#if BUNGEEGUM_USE_OVERLAY
                container.push_back(_exception);
#else
                    console_log_error(_exception);
                    (void)container;
#endif
            });
        }
    }

    widget_id widget_id_access::make_from_data(const std::uintptr_t& data)
    {
        return widget_id(data);
    }

    std::uintptr_t widget_id_access::get_data(const widget_id& id)
    {
        return id._data;
    }

    draw_command draw_command_access::make_from_data(const draw_command_data& data)
    {
        return draw_command(data);
    }

    draw_command_data& draw_command_access::get_data(draw_command& command)
    {
        return command._data;
    }

    resolve_command resolve_command_access::make_from_data(const resolve_command_data& data)
    {
        return resolve_command(data);
    }

    resolve_command_data& resolve_command_access::get_data(resolve_command& command)
    {
        return command._data;
    }

    interact_command interact_command_access::make_from_data(const interact_command_data& data)
    {
        return interact_command(data);
    }

    interact_command_data& interact_command_access::get_data(interact_command& command)
    {
        return command._data;
    }

    // bool widget_manager_data::contains(const std::uintptr_t raw_widget) const
    // {
    //     return _updatables.find(raw_widget) != _updatables.end();
    // }

    // void widget_manager_data::notify_erase(const std::uintptr_t raw_widget)
    // {
    //     _updatables_to_erase.push_back(raw_widget);
    // }

    // widget_update_data& widget_manager_data::operator[](const std::uintptr_t raw_widget)
    // {
    //     return _updatables[raw_widget];
    // }

    // widget_update_data& widget_manager_data::operator[](const widget_id& widget)
    // {
    //     return _updatables[raw(widget)];
    // }

    // std::uintptr_t widget_manager_data::raw(const widget_id& runtime_widget)
    // {
    //     return runtime_widget._data.raw_widget;
    // }

    // std::uintptr_t& widget_manager_data::root()
    // {
    //     return _root;
    // }

    // std::uintptr_t widget_manager_data::root() const
    // {
    //     return _root;
    // }

    // widget_update_data& widget_manager_data::root_update_data()
    // {
    //     return _updatables[_root];
    // }

    // const widget_update_data& widget_manager_data::root_update_data() const
    // {
    //     return _updatables.insert_or_assign(_root);
    // }

    // widget_id widget_manager_data::create_runtime_widget(widget_update_data& widget_data)
    // {
    //     return widget_id(detail::widget_id_data { widget_data.raw_widget });
    // }

    // void widget_manager_data::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback)
    // {
    //     std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
    //         bool _continue = iterate_callback(_update_data);
    //         if (_continue) {
    //             for (auto& _child : _update_data.children) {
    //                 _iterate(_child.get());
    //             }
    //         }
    //     };
    //     _iterate(iterate_root);
    // }

    // void widget_manager_data::traverse(widget_update_data& iterate_root, const std::function<bool(widget_update_data&)>& iterate_callback,
    //     const std::function<void(widget_update_data&)>& tail_iterate_callback)
    // {
    //     std::function<void(widget_update_data&)> _iterate = [&](widget_update_data& _update_data) {
    //         bool _continue = iterate_callback(_update_data);
    //         if (_continue) {
    //             for (auto& _child : _update_data.children) {
    //                 _iterate(_child.get());
    //             }
    //         }
    //         tail_iterate_callback(_update_data);
    //     };
    //     _iterate(iterate_root);
    // }
}

interaction_type interact_command::type() const
{
    size_t _index = _data.get().index();
    return static_cast<interaction_type>(_index);
}

#define get_interaction_impl(interaction_name)                                                             \
    std::optional<interaction_name##_interaction> interact_command::interaction_name() const               \
    {                                                                                                      \
        size_t _index = static_cast<size_t>(interaction_type::interaction_name);                           \
        if (_index != _data.get().index())                                                                 \
            return std::nullopt;                                                                           \
        detail::interaction_name##_event _event = std::get<detail::interaction_name##_event>(_data.get()); \
        interaction_name##_interaction _interaction;                                                       \
        _interaction._event = _event;                                                                      \
        return std::make_optional<interaction_name##_interaction>(_interaction);                           \
    }

get_interaction_impl(window_resized);
get_interaction_impl(mouse_moved);
get_interaction_impl(mouse_down);
get_interaction_impl(mouse_up);
get_interaction_impl(mouse_pressed);

#undef get_interaction_impl


interact_command::interact_command(const detail::interact_command_data& data)
    : _data(data)
{
}

void interact_command::interact_children()
{
}

resolve_command::resolve_command(const detail::resolve_command_data& data)
    : _data(data)
{
}

float2 resolve_command::get_min_size() const
{
    return _data.get().local_min_size;
}

float2 resolve_command::get_max_size() const
{
    return _data.get().local_max_size;
}

void resolve_command::resize(const float2 size)
{
    _data.get().resolved_size = size;
}

float2 resolve_command::resolve_child(const widget_id child_id, const float2 min_size, const float2 max_size) const
{
    detail::global_manager_data& _global = detail::global();
    const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _child_updatable = _global.widgets.updatables[_child_raw];
    detail::widget_update_data& _updatable = _data.get();
    _child_updatable.local_min_size = min_size;
    _child_updatable.local_max_size = max_size;
#if BUNGEEGUM_USE_OVERLAY
    _global.pipelines.current.value().get().widgets_chronometer.end_task(_updatable.clean_typename);
    _global.pipelines.current.value().get().widgets_chronometer.begin_task(_child_updatable.clean_typename);
#endif
    detail::protect_userspace(_global.logs.userspace_errors, [&_child_updatable]() {
        resolve_command _command = detail::resolve_command_access::make_from_data(_child_updatable);
        _child_updatable.resolver(_command);
    });
#if BUNGEEGUM_USE_OVERLAY
    _global.pipelines.current.value().get().widgets_chronometer.end_task(_child_updatable.clean_typename);
    _global.pipelines.current.value().get().widgets_chronometer.begin_task(_updatable.clean_typename);
#endif
    return _child_updatable.resolved_size;
}

void resolve_command::position_child(const widget_id child_id, const float2 position)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    _child_updatable.local_position = float3(position, 0.f);
}

// void draw_command_data::draw()
// {
//     ImDrawList* _draw_list = ImGui::GetBackgroundDrawList();
//     draw(_draw_list);
// }

// void draw_command_data::draw(ImDrawList* imgui_drawlist)
// {
//     for (auto& _command : commands)
//         _command(imgui_drawlist);
// }


draw_command::draw_command(const detail::draw_command_data& data)
    : _data(data)
{
}

float2 draw_command::get_size() const
{
    return _data.updatable.get().resolved_size;
}

void draw_command::draw_children()
{
}

void draw_command::draw_line(
    const float2 first_point, const float2 second_point,
    const float4 color,
    const float thickness)
{
    ImVec2 _first_point { first_point.x, first_point.y };
    ImVec2 _second_point { second_point.x, second_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.list->AddLine(_first_point, _second_point, _color, thickness);
}

void draw_command::draw_rect(
    const float2 min_point, const float2 max_point,
    const float4 color,
    const float rounding,
    const float thickness)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.list->AddRect(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All, thickness);
}

void draw_command::draw_rect_filled(
    const float2 min_point, const float2 max_point,
    const float4 color,
    const float rounding)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.list->AddRectFilled(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All);
}

widget_id::widget_id(const detail::widget_id_data& data)
    : _data(data)
{
}

void adopt(const widget_id parent_id, const widget_id child_id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _parent_raw = detail::widget_id_access::get_data(parent_id);
    const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _parent_updatable = _manager.updatables[_parent_raw];
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    _child_updatable.parent = _parent_updatable;
    _parent_updatable.children.emplace_back(_child_updatable);
}

void abandon(const widget_id parent_id, const widget_id child_id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _parent_raw = detail::widget_id_access::get_data(parent_id);
    const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _parent_updatable = _manager.updatables[_parent_raw];
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    _child_updatable.parent = std::nullopt;
    for (std::size_t _index = 0; _index < _parent_updatable.children.size(); _index++) {
        detail::widget_update_data& _loop_updatable = _parent_updatable.children[_index].get();
        if (_loop_updatable.raw == _child_updatable.raw) {
            _parent_updatable.children.erase(_parent_updatable.children.begin() + _index);
            break;
        }
    }
}

void destroy(const widget_id& widget)
{
    (void)widget;
    // ?
    // ?
    // ?
    // ?
    // void* _void_widget = reinterpret_cast<void*>(&widget);
    // detail::registry_entity _entity;
    // if (detail::global().widgets->.accessors.find(_void_widget) == detail::global().widgets->.accessors.end())
    //     detail::throw_error<detail::error_type::bad_implementation>("widget not found in accessors");
    // _entity = detail::global().widgets->.possessed.at(_void_widget);
    // detail::global().widgets->.widgets.destroy_entity(_entity);
    // detail::global().widgets->.possessed.erase(_void_widget);
    // detail::global().widgets->.accessors.erase(_void_widget);
}

widget_id get_parent(const widget_id id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
    if (!_updatable.parent.has_value()) {
        // log_error("Error TODO");
    }
    detail::widget_update_data& _parent_updatable = _updatable.parent.value();
    return detail::widget_id_access::make_from_data(_parent_updatable.raw);
}

bool has_parent(const widget_id id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
    return _updatable.parent.has_value();
}

void must_resolve(const widget_id id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
    _manager.resolvables.push_back(_updatable);
}

void must_draw(const widget_id id)
{
    const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_manager_data& _manager = detail::global().widgets;
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
    _manager.drawables.push_back(_updatable);
}

}