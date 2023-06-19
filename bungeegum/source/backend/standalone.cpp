#pragma once

#include <bungeegum/backend/standalone.hpp>

namespace bungeegum {
namespace detail {

    void update_desired_data(window& current_window)
    {
        if (detail::is_desired_app_color_changed) {
            // todo bizarre
            detail::is_desired_app_color_changed = false;
        }
        if (detail::is_desired_app_title_changed) {
            current_window.set_title(desired_app_title);
            detail::is_desired_app_title_changed = false;
        }
    }

}

void bungeegum_app::color(const float4 rgba)
{
    detail::is_desired_app_color_changed = true;
    detail::desired_app_color = rgba;
}

void bungeegum_app::title(const std::string& description)
{
    detail::is_desired_app_title_changed = true;
    detail::desired_app_title = description;
}

float2 bungeegum_app::viewport()
{
    return detail::viewport_size;
}
}
