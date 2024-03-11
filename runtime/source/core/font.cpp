#include <bungeegum/core/font.hpp>
#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

    namespace {

        void reset_fonts(font_data& data)
        {
            for (std::pair<const std::uintptr_t, font_handle>& _it : data.fonts) {
                _it.second.reset();
            }
            data.fonts.clear();
        }
    }

    const font_data& font_access::get_data(const font& object)
    {
        return object._data;
    }
    
    font_data::font_data()
    {
        raw = raw_cast(this); // create a new id
    }

    font_data::font_data(const font_data& other)
    {
        *this = other;
    }

    font_data& font_data::operator=(const font_data& other)
    {
        reset_fonts(*this);
        raw = raw_cast(this); // create a new id
        is_compiled = other.is_compiled;
        creation_ttf = other.creation_ttf;
        creation_count = other.creation_count;
        creation_size = other.creation_size;
        if (is_compiled) {
            swapped_manager_data& _swapped = swapped_global();
            for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::rasterizer_handle>> _pipeline : _swapped.rasterizers) { 
                fonts[_pipeline.first].emplace(
                    _pipeline.second.get(),
                    creation_ttf,
                    creation_count,
                    creation_size);
            }
        }
        return *this;
    }

    font_data::font_data(font_data&& other)
    {
        *this = std::move(other);
    }

    font_data& font_data::operator=(font_data&& other)
    {        
        reset_fonts(*this);
        raw = other.raw; // keep the same id
        is_compiled = std::move(other.is_compiled);
        creation_ttf = std::move(other.creation_ttf);
        creation_count = std::move(other.creation_count);
        creation_size = std::move(other.creation_size);
        fonts = std::move(other.fonts); // just move handles
        return *this;
    }

    font_data::~font_data()
    {
        reset_fonts(*this);
    }
}


font::font(const std::filesystem::path& filename, const float size)
{
    detail::swapped_manager_data& _swapped = detail::swapped_global();
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::rasterizer_handle>> _pipeline : _swapped.rasterizers) { 
        _data.fonts[_pipeline.first].emplace(
            _pipeline.second.get(),
            filename,
            size);
    }
}

font::font(const void* ttf, const std::size_t count, const float size)
{
    detail::swapped_manager_data& _swapped = detail::swapped_global();
    for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::rasterizer_handle>> _pipeline : _swapped.rasterizers) { 
        _data.fonts[_pipeline.first].emplace(
            _pipeline.second.get(),
            ttf,
            count,
            size);
    }
}

}