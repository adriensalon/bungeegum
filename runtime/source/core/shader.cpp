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

    const shader_resource_data& shader_resource_access::get_data(const shader_resource& object)
    {
        return object._data;
    }

    shader shader_access::make_from_data(const shader_data& data)
    {
        return shader(data);
    }

    shader_data& shader_access::get_data(shader& object)
    {
        return object._data;
    }

    const shader_data& shader_access::get_data(const shader& object)
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

// shader_resource& shader_resource::blend(const shader_blend_options& options)
// {
//     _data.blend = options._data.options;
//     return *this;
// }





template <>
shader& shader::uniform<float>(const std::string& name, const float& value)
{
    return *this;
}

shader::shader(const detail::shader_data& data)
    : _data(data)
{
}

shader& shader::emplace(const shader_resource& resource)
{
    detail::global_manager_data& _global = detail::global();
    const detail::shader_resource_data& _resource_data = detail::shader_resource_access::get_data(resource);
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        detail::pipeline_data& _pipeline_data = _it.second.get();
        _data.shaders[_it.first].emplace(
            _pipeline_data.user_context,
            _resource_data.fragment,
            _resource_data.blend);
    }
    return *this;
}

}