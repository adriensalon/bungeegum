#include <bungeegum/core/global.fwd>
#include <bungeegum/core/texture.hpp>

namespace bungeegum {
namespace detail {

    const texture_data& texture_access::get_data(const texture& object)
    {
        return object._data;
    }
}

texture& texture::compile(const std::vector<unsigned char>& pixels, const uint2 size)
{
    detail::global_manager_data& _global = detail::global();
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        detail::pipeline_data& _pipeline_data = _it.second.get();
		_data.textures[_it.first].emplace(
			_pipeline_data.user_context, 
			pixels, 
			static_cast<std::size_t>(size.x), 
			static_cast<std::size_t>(size.y));
    }
    return *this;
        // texture_view = pFontTex->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        // rnd._diligent_shader_resource.Release();
        // rnd._diligent_ignore_stencil_pipeline->CreateShaderResourceBinding(&(rnd._diligent_shader_resource), true);
        // rnd._diligent_texture_variable = rnd._diligent_shader_resource->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Texture");
        // VERIFY_EXPR(rnd._diligent_texture_variable != nullptr);
}

}