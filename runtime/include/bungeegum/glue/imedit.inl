#pragma once

#include <type_traits>

#include <imgui.h>

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value)
    {
        if constexpr (std::is_same_v<value_t, float1>) {
            ImGui::DragFloat(name.c_str(), &value);
        }
    }

}
}
