#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/resolve.hpp>
#include <bungeegum/core/widget.hpp>

namespace bungeegum {

float2 resolve_command::min_size() const
{
    return _data.constraint.min_size;
}

float2 resolve_command::max_size() const
{
    return _data.constraint.max_size;
}

void resolve_command::resize(const float2 size)
{
    _data.resolved_size = size;
}


float2 resolve_command::resolve_child(const widget_id& child_widget, const float2 min_size, const float2 max_size) const
{
	detail::widgets_registry_data& _registry = detail::global().widgets.registry;
	detail::widget_id_data _child_id = detail::widget_id_access::get_data(child_widget);
    detail::widget_connector_data& _child_connector = _registry.get_updatable(_child_id)->connector;
    resolve_command& _child_resolve_command = _child_connector.get_resolve_command();
    _child_resolve_command._data.constraint.min_size = min_size;
    _child_resolve_command._data.constraint.max_size = max_size;
#if BUNGEEGUM_USE_OVERLAY
    // if (_data.parent_clean_typename.has_value()) {
    //     detail::global().backend.profiler_resolve_chronometer.end_task(_data.parent_clean_typename.value());
    // }
    detail::global().pipelines.profiler_resolve_chronometer.end_task(_data.clean_typename);
    detail::global().pipelines.profiler_resolve_chronometer.begin_task(_child_resolve_command._data.clean_typename);
#endif
    detail::global().logs.protect_userspace([&_child_connector]() {
        _child_connector.resolve();
    });
#if BUNGEEGUM_USE_OVERLAY
    detail::global().pipelines.profiler_resolve_chronometer.end_task(_child_resolve_command._data.clean_typename);
    detail::global().pipelines.profiler_resolve_chronometer.begin_task(_data.clean_typename);
    // if (_data.parent_clean_typename.has_value()) {
    //     detail::global().backend.profiler_resolve_chronometer.begin_task(_data.parent_clean_typename.value());
    // }
#endif
    return _child_resolve_command._data.resolved_size;
}

void resolve_command::position_child(const widget_id& child_widget, const float2 position)
{
	detail::widgets_registry_data& _registry = detail::global().widgets.registry;
	detail::widget_id_data _child_id = detail::widget_id_access::get_data(child_widget);
    detail::widget_connector_data& _child_connector = _registry.get_updatable(_child_id)->connector;

    resolve_command& _child_resolve_command = _child_connector.get_resolve_command();
    _child_resolve_command._data.resolved_position = position;
}

void must_resolve(const widget_id& widget)
{
    detail::widget_id_data _id = detail::widget_id_access::get_data(widget);
    std::unique_ptr<detail::widget_update_data>& _updatable = detail::global().widgets.registry.get_updatable(_id);
    detail::global().widgets.processor.must_resolve(_updatable);
}
}