#include <bungeegum/core/global.fwd>
#include <bungeegum/core/shader.hpp>

namespace bungeegum {
namespace detail {

    shader_resource shader_resource_access::make_from_data(const shader_resource_data& data)
    {
        return shader_resource(data);
    }

    shader_resource_data& shader_resource_access::get_data(shader_resource& object)
    {
        return object._data;
    }

    shader_ref shader_ref_access::make_from_data(const shader_ref_data& data)
    {
        return shader_ref(data);
    }

    shader_ref_data& shader_ref_access::get_data(shader_ref& object)
    {
        return object._data;
    }
}

shader_resource::shader_resource(const detail::shader_resource_data& data)
    : _data(data)
{
}

shader_resource& shader_resource::fragment(const std::string& source, std::initializer_list<int> args)
{
    _data.fragment = source;
    return *this;
}

shader_resource& shader_resource::depth(const shader_depth_options& options)
{

    return *this;
}

shader_resource& shader_resource::stencil(const shader_stencil_options& options)
{
    // _data.stencil = options._data;
    return *this;
}

template <>
shader_ref& shader_ref::uniform<float>(const std::string& name, const float& value)
{
    return *this;
}

shader_ref::shader_ref(const detail::shader_ref_data& data)
    : _data(data)
{
}

shader_ref get_shader(const std::string& name)
{
    detail::global_manager_data& _global = detail::global();
    detail::shader_ref_data& _data = _global.shaders.shaders[name];
    return detail::shader_ref_access::make_from_data(_data);
}

shader_ref make_shader(const std::string& name, shader_resource& resource)
{
    detail::global_manager_data& _global = detail::global();
    detail::shader_ref_data& _ref_data = _global.shaders.shaders[name];
    detail::shader_resource_data& _resource_data = detail::shader_resource_access::get_data(resource);
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        detail::pipeline_data& _pipeline_data = _it.second.get();
        _ref_data.shaders[_it.first].create(
            _pipeline_data.pipeline_renderer,
            _resource_data.fragment,
            _resource_data.blend,
            _resource_data.depth,
            _resource_data.stencil);
    }
    return detail::shader_ref_access::make_from_data(_ref_data);
}

}