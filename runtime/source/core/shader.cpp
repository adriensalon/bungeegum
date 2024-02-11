#include <bungeegum/core/global.fwd>
#include <bungeegum/core/shader.hpp>

namespace bungeegum {
namespace detail {

	shader_ref shader_ref_access::make_from_data(const shader_ref_data& data)
	{
		return shader_ref(data);
	}

	shader_ref_data& shader_ref_access::get_data(shader_ref& object)
	{
		return object._data;
	}
}

shader_resource& shader_resource::fragment(const std::string& source, std::initializer_list<int> args)
{
    _data.fragment = source;
    return *this;
}

shader_resource& shader_resource::blend(const shader_blend_options& options)
{

    return *this;
}

shader_resource& shader_resource::stencil(const shader_stencil_options& options)
{

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
	detail::shader_ref_data& _data = _global.shaders.shaders[name];
	for (std::pair<const uintptr_t, std::reference_wrapper<bungeegum::detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
		detail::shader_view& _rasterizer = _data.rasterizers[_it.first];
		// todo setup every rasterizer (1 for each pipeline)
		(void)_rasterizer;
	}
    return detail::shader_ref_access::make_from_data(_data);
}

}