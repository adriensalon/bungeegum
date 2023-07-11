#pragma once

#include <bungeegum/glue/window.hpp>

namespace bungeegum {
namespace detail {

    inline bool is_desired_app_color_changed = false;
    inline float4 desired_app_color = { 1.f, 1.f, 1.f, 1.f };
    inline bool is_desired_app_title_changed = false;
    inline std::string desired_app_title = "Bungeegum App";

    void update_desired_data(window& current_window);
}
}
