#include <bungeegum/core/font.hpp>
#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

    const font_data& font_access::get_data(const font& object)
    {
        return object._data;
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