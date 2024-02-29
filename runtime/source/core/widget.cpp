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

void resolve_command::position_child(const widget_id child_id, const float2 position, const bool absolute)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _child_raw = detail::widget_id_access::get_data(child_id);
    detail::widget_update_data& _child_updatable = _manager.updatables[_child_raw];
    if (absolute) {
        _child_updatable.local_position = zero<float3>;
    }    
    _child_updatable.local_position += float3(position, 0.f);
}











draw_command::draw_command(const detail::draw_command_data& data)
    : _data(data)
{
}

float2 draw_command::get_size() const
{
    return _data.updatable.get().resolved_size;
}

void draw_command::draw_child(const widget_id child_id)
{
    
}

void draw_command::draw_children()
{
}

void draw_command::draw_curve()
{
}

void draw_command::draw_curve_filled()
{
}

void draw_command::draw_texture(texture_ref& user_texture, const float2 min_point, const float2 max_point)
{
    const detail::texture_ref_data& _texture_ref_data = detail::texture_ref_access::get_data(user_texture);
    const detail::texture_handle& _texture_handle = _texture_ref_data.textures.at(_data.raw_pipeline);
    ImTextureID _id = _texture_handle.get();
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    _data.draw_list->AddImage(_id, _min_point, _max_point);
}

void draw_command::draw_texture_quad(texture_ref& user_texture, const float2 top_left_corner, const float2 top_right_corner, const float2 bottom_right_corner, const float2 bottom_left_corner)
{
    const detail::texture_ref_data& _texture_ref_data = detail::texture_ref_access::get_data(user_texture);
    const detail::texture_handle& _texture_handle = _texture_ref_data.textures.at(_data.raw_pipeline);
    ImTextureID _id = _texture_handle.get();
    ImVec2 _top_left_corner { top_left_corner.x, top_left_corner.y };
    ImVec2 _top_right_corner { top_right_corner.x, top_right_corner.y };
    ImVec2 _bottom_left_corner { bottom_left_corner.x, bottom_left_corner.y };
    ImVec2 _bottom_right_corner { bottom_right_corner.x, bottom_right_corner.y };
    _data.draw_list->AddImageQuad(_id, _top_left_corner, _top_right_corner, _bottom_right_corner, _bottom_left_corner);
}

void draw_command::draw_texture_rounded(texture_ref& user_texture, const float2 min_point, const float2 max_point, const float rounding)
{
    const detail::texture_ref_data& _texture_ref_data = detail::texture_ref_access::get_data(user_texture);
    const detail::texture_handle& _texture_handle = _texture_ref_data.textures.at(_data.raw_pipeline);
    ImTextureID _id = _texture_handle.get();
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    _data.draw_list->AddImageRounded(_id, _min_point, _max_point, ImVec2(0.f, 0.f), ImVec2(0.f, 0.f), 4294967295U, rounding, ImDrawCornerFlags_All);
}

void draw_command::draw_text(font_ref& user_font, const float font_size, const float2 position, const float4 color, const std::string& text)
{
    const detail::font_ref_data& _font_ref_data = detail::font_ref_access::get_data(user_font);
    const detail::font_handle& _font_handle = _font_ref_data.fonts.at(_data.raw_pipeline);
    ImFont* _font = _font_handle.get();
    const char* _ctext = text.c_str();
    ImVec2 _position { position.x, position.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.draw_list->AddText(_font, font_size, _position, _color, _ctext);
}

void draw_command::draw_line(const float2 first_point, const float2 second_point, const float4 color, const float thickness)
{
    ImVec2 _first_point { first_point.x, first_point.y };
    ImVec2 _second_point { second_point.x, second_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.draw_list->AddLine(_first_point, _second_point, _color, thickness);
}

void draw_command::draw_rect(const float2 min_point, const float2 max_point, const float4 color, const float rounding, const float thickness)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.draw_list->AddRect(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All, thickness);
}

void draw_command::draw_rect_filled(const float2 min_point, const float2 max_point, const float4 color, const float rounding)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });
    _data.draw_list->AddRectFilled(_min_point, _max_point, _color, rounding, ImDrawCornerFlags_All);
}

void draw_command::draw_rect_filled_multi_color(const float2 min_point, const float2 max_point, const float4 color_top_left, const float4 color_top_right, const float4 color_bottom_right, const float4 color_bottom_left)
{
    ImVec2 _min_point { min_point.x, min_point.y };
    ImVec2 _max_point { max_point.x, max_point.y };
    ImU32 _color_top_left = ImGui::ColorConvertFloat4ToU32({ color_top_left.x, color_top_left.y, color_top_left.z, color_top_left.w });
    ImU32 _color_top_right = ImGui::ColorConvertFloat4ToU32({ color_top_right.x, color_top_right.y, color_top_right.z, color_top_right.w });
    ImU32 _color_bottom_right = ImGui::ColorConvertFloat4ToU32({ color_bottom_right.x, color_bottom_right.y, color_bottom_right.z, color_bottom_right.w });
    ImU32 _color_bottom_left = ImGui::ColorConvertFloat4ToU32({ color_bottom_left.x, color_bottom_left.y, color_bottom_left.z, color_bottom_left.w });
    _data.draw_list->AddRectFilledMultiColor(_min_point, _max_point, _color_top_left, _color_top_right, _color_bottom_right, _color_bottom_left);
}

void draw_command::draw_quad(const float2 top_left_corner, const float2 top_right_corner, const float2 bottom_right_corner, const float2 bottom_left_corner, const float4 color, const float thickness)
{
    ImVec2 _top_left_corner { top_left_corner.x, top_left_corner.y };
    ImVec2 _top_right_corner { top_right_corner.x, top_right_corner.y };
    ImVec2 _bottom_left_corner { bottom_left_corner.x, bottom_left_corner.y };
    ImVec2 _bottom_right_corner { bottom_right_corner.x, bottom_right_corner.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });    
    _data.draw_list->AddQuad(_top_left_corner, _top_right_corner, _bottom_right_corner, _bottom_left_corner, _color, thickness);
}

void draw_command::draw_quad_filled(const float2 top_left_corner, const float2 top_right_corner, const float2 bottom_right_corner, const float2 bottom_left_corner, const float4 color)
{
    ImVec2 _top_left_corner { top_left_corner.x, top_left_corner.y };
    ImVec2 _top_right_corner { top_right_corner.x, top_right_corner.y };
    ImVec2 _bottom_left_corner { bottom_left_corner.x, bottom_left_corner.y };
    ImVec2 _bottom_right_corner { bottom_right_corner.x, bottom_right_corner.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });    
    _data.draw_list->AddQuadFilled(_top_left_corner, _top_right_corner, _bottom_right_corner, _bottom_left_corner, _color);
}

void draw_command::draw_triangle(const float2 first_corner, const float2 second_corner, const float2 third_corner, const float4 color, const float thickness)
{
    ImVec2 _first_corner { first_corner.x, first_corner.y };
    ImVec2 _second_corner { second_corner.x, second_corner.y };
    ImVec2 _third_corner { third_corner.x, third_corner.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });    
    _data.draw_list->AddTriangle(_first_corner, _second_corner, _third_corner, _color, thickness);
}

void draw_command::draw_triangle_filled(const float2 first_corner, const float2 second_corner, const float2 third_corner, const float4 color)
{
    ImVec2 _first_corner { first_corner.x, first_corner.y };
    ImVec2 _second_corner { second_corner.x, second_corner.y };
    ImVec2 _third_corner { third_corner.x, third_corner.y };
    ImU32 _color = ImGui::ColorConvertFloat4ToU32({ color.x, color.y, color.z, color.w });    
    _data.draw_list->AddTriangleFilled(_first_corner, _second_corner, _third_corner, _color);
}

void use_shader_callback(const ImDrawList* parent_list, const ImDrawCmd* cmd) 
{
    detail::shader_handle _shader;
    _shader.emplace(cmd->UserCallbackData);    
    detail::global_manager_data& _global = detail::global();
    detail::pipeline_data& _pipeline_data = _global.pipelines.current.value().get();    
    _pipeline_data.user_context.use_shader(_shader);
}

void draw_command::use_shader_custom(const shader& user_shader)
{
    const detail::shader_data& _shader_data = detail::shader_access::get_data(user_shader);
    const detail::shader_handle& _shader_handle = _shader_data.shaders.at(_data.raw_pipeline);  
    _data.draw_list->AddCallback(use_shader_callback, _shader_handle.get());
}

void draw_command::use_shader_default()
{
    detail::global_manager_data& _global = detail::global();
    detail::pipeline_data& _pipeline_data = _global.pipelines.current.value().get();  
    _data.draw_list->AddCallback(use_shader_callback, _pipeline_data.default_shader.get());
}

void draw_command::use_shader_mask()
{
    detail::global_manager_data& _global = detail::global();
    detail::pipeline_data& _pipeline_data = _global.pipelines.current.value().get();
    _data.draw_list->AddCallback(use_shader_callback, _pipeline_data.mask_shader.get());
}

void draw_command::clear_mask()
{
    
}

void draw_command::use_projection_orthographic()
{

}

void draw_command::use_projection_perspective(const float fov)
{

}

void draw_command::use_scissors(const float2 first_point, const float2 second_point, const bool keep)
{

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
    if (_manager.resolvables.find(_raw) != _manager.resolvables.end()) {
		_manager.resolvables.insert(std::make_pair(_raw, std::ref(_updatable)));
	}
}

void must_draw(const widget_id id)
{
    detail::widget_manager_data& _manager = detail::global().widgets;
    const std::uintptr_t _raw = detail::widget_id_access::get_data(id);
    detail::widget_update_data& _updatable = _manager.updatables[_raw];
	if (_manager.drawables.find(_raw) != _manager.drawables.end()) {
		_manager.drawables.insert(std::make_pair(_raw, std::ref(_updatable)));
	}
}

}