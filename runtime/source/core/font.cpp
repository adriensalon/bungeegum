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
            detail::global_manager_data& _global = detail::global();
            for (std::pair<const std::uintptr_t, std::reference_wrapper<detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
                detail::pipeline_data& _pipeline_data = _it.second.get();        
                fonts[_it.first].emplace(
                    _pipeline_data.user_context,
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


font& font::compile(const void* ttf, const std::size_t count, const float size)
{
    detail::global_manager_data& _global = detail::global();
    for (std::pair<const uintptr_t, std::reference_wrapper<bungeegum::detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        detail::pipeline_data& _pipeline_data = _it.second.get();
        _data.fonts[_it.first].emplace(
            _pipeline_data.user_context,
            ttf,
            count,
            size);
    }
    return *this;
}

}