#include <bungeegum/core/global.fwd>
#include <bungeegum/core/texture.hpp>

namespace bungeegum {
namespace detail {

    namespace {
        
        void reset_textures(texture_data& data)
        {
            for (std::pair<const std::uintptr_t, texture_handle>& _it : data.textures) {
                _it.second.reset();
            }
            data.textures.clear();
        }

    }

    const texture_data& texture_access::get_data(const texture& object)
    {
        return object._data;
    }

    texture_data::texture_data()
    {
        raw = raw_cast(this); // create a new id
    }

    texture_data::texture_data(const texture_data& other)
    {
        *this = other;
    }

    texture_data& texture_data::operator=(const texture_data& other)
    {
        reset_textures(*this);
        raw = raw_cast(this); // create a new id
        is_compiled = other.is_compiled;
        creation_filename = other.creation_filename;
        creation_pixels = other.creation_pixels;
        creation_size = other.creation_size;
        if (is_compiled) {
            detail::global_manager_data& _global = detail::global();
            for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
                detail::pipeline_data& _pipeline_data = _it.second.get();   
                if (creation_filename.empty()) {
                    textures[_it.first].emplace(
                        _pipeline_data.user_context,
                        creation_pixels,
                        creation_size.x,
                        creation_size.y);
                } else {
                    textures[_it.first].emplace(
                        _pipeline_data.user_context,
                        creation_filename);
                }
            }
        }
        return *this;
    }

    texture_data::texture_data(texture_data&& other)
    {
        *this = std::move(other);
    }

    texture_data& texture_data::operator=(texture_data&& other)
    {        
        reset_textures(*this);
        raw = other.raw; // keep the same id
        is_compiled = std::move(other.is_compiled);
        creation_filename = std::move(other.creation_filename);
        creation_pixels = std::move(other.creation_pixels);
        creation_size = std::move(other.creation_size);
        textures = std::move(other.textures); // just move handles
        return *this;
    }

    texture_data::~texture_data()
    {
        reset_textures(*this);
    }
}

texture& texture::compile(const std::filesystem::path& filename)
{
    detail::global_manager_data& _global = detail::global();
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        detail::pipeline_data& _pipeline_data = _it.second.get();
		_data.textures[_it.first].emplace(
			_pipeline_data.user_context, 
			filename);
    }
    _data.creation_filename = filename;
    return *this;
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
    _data.creation_pixels = pixels;
    _data.creation_size = size;
    return *this;
        // texture_view = pFontTex->GetDefaultView(Diligent::TEXTURE_VIEW_SHADER_RESOURCE);
        // rnd._diligent_texture_resource.Release();
        // rnd._diligent_ignore_stencil_pipeline->CreateShaderResourceBinding(&(rnd._diligent_texture_resource), true);
        // rnd._diligent_texture_variable = rnd._diligent_texture_resource->GetVariableByName(Diligent::SHADER_TYPE_PIXEL, "Texture");
        // VERIFY_EXPR(rnd._diligent_texture_variable != nullptr);
}

}