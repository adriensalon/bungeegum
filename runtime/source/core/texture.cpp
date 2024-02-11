#include <bungeegum/core/global.fwd>
#include <bungeegum/core/texture.hpp>

namespace bungeegum {
namespace detail {

    texture_ref texture_ref_access::make_from_data(const texture_ref_data& data)
    {
        return texture_ref(data);
    }

    texture_ref_data& texture_ref_access::get_data(texture_ref& object)
    {
        return object._data;
    }

    texture_resource texture_resource_access::make_from_data(const texture_resource_data& data)
    {
        return texture_resource(data);
    }

    texture_resource_data& texture_resource_access::get_data(texture_resource& object)
    {
        return object._data;
    }
}

texture_resource& texture_resource::pixels(const std::vector<unsigned char>& char_pixels)
{
    _data.pixels = char_pixels;
    return *this;
}

texture_resource& texture_resource::size(const std::size_t height, const std::size_t width)
{
    _data.width = width;
    _data.height = height;
    return *this;
}

texture_resource::texture_resource(const detail::texture_resource_data& data)
    : _data(data)
{
}

texture_ref::texture_ref(const detail::texture_ref_data& data)
    : _data(data)
{
}

texture_ref get_texture(const std::string& name)
{
    detail::global_manager_data& _global = detail::global();
    detail::texture_ref_data& _ref_data = _global.textures.textures[name];
    return detail::texture_ref_access::make_from_data(_ref_data);
}

texture_ref make_texture(const std::string& name, texture_resource& resource)
{
    detail::global_manager_data& _global = detail::global();
    detail::texture_ref_data& _ref_data = _global.textures.textures[name];
    detail::texture_resource_data& _resource_data = detail::texture_resource_access::get_data(resource);
    for (std::pair<const uintptr_t, std::reference_wrapper<bungeegum::detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        // detail::renderer& _renderer = _it.second.get().pipeline_renderer;
		(void)_it;
        Diligent::TextureDesc FontTexDesc;
        FontTexDesc.Name = "Imgui font texture";
        FontTexDesc.Type = Diligent::RESOURCE_DIM_TEX_2D;
        FontTexDesc.Width = static_cast<Diligent::Uint32>(_resource_data.width);
        FontTexDesc.Height = static_cast<Diligent::Uint32>(_resource_data.height);
        FontTexDesc.Format = Diligent::TEX_FORMAT_RGBA8_UNORM;
        FontTexDesc.BindFlags = Diligent::BIND_SHADER_RESOURCE;
        FontTexDesc.Usage = Diligent::USAGE_IMMUTABLE;

        // Diligent::TextureSubResData Mip0Data[] = { { _resource_data.pixels.data(), 4 * Diligent::Uint64 { FontTexDesc.Width } } };
        // Diligent::TextureData InitData(Mip0Data, _countof(Mip0Data));

        // Diligent::RefCntAutoPtr<Diligent::ITexture> pFontTex;
        // rnd._diligent_render_device->CreateTexture(FontTexDesc, &InitData, &pFontTex);

        // texture_view = pFontTex->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        // rnd._diligent_shader_resource.Release();
        // rnd._diligent_ignore_stencil_pipeline->CreateShaderResourceBinding(&(rnd._diligent_shader_resource), true);
        // rnd._diligent_texture_variable = rnd._diligent_shader_resource->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Texture");
        // VERIFY_EXPR(rnd._diligent_texture_variable != nullptr);
    }
    return detail::texture_ref_access::make_from_data(_ref_data);
}

}