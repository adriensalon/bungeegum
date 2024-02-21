#include <bungeegum/core/font.hpp>
#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

    font_ref font_ref_access::make_from_data(const font_ref_data& data)
    {
        return font_ref(data);
    }

    font_ref_data& font_ref_access::get_data(font_ref& object)
    {
        return object._data;
    }

    font_resource font_resource_access::make_from_data(const font_resource_data& data)
    {
        return font_resource(data);
    }

    font_resource_data& font_resource_access::get_data(font_resource& object)
    {
        return object._data;
    }
}

font_resource::font_resource(const detail::font_resource_data& data)
    : _data(data)
{   
}

font_ref::font_ref(const detail::font_ref_data& data)
    : _data(data)
{
}

font_ref get_font(const std::string& name)
{
    detail::global_manager_data& _global = detail::global();
    detail::font_ref_data& _ref_data = _global.fonts.fonts[name];
    return detail::font_ref_access::make_from_data(_ref_data);
}

font_ref make_font(const std::string& name, font_resource& resource)
{
    detail::global_manager_data& _global = detail::global();
    detail::font_ref_data& _ref_data = _global.fonts.fonts[name];
    detail::font_resource_data& _resource_data = detail::font_resource_access::get_data(resource);
    for (std::pair<const uintptr_t, std::reference_wrapper<bungeegum::detail::pipeline_data>>& _it : _global.pipelines.pipelines) {
        detail::pipeline_data& _pipeline_data = _it.second.get();
        _ref_data.fonts[_it.first].create(
            _pipeline_data.pipeline_renderer,
            _pipeline_data.user_context,
            _resource_data.compressed,
            _resource_data.count,
            _resource_data.size);
    }
    return detail::font_ref_access::make_from_data(_ref_data);
}

}