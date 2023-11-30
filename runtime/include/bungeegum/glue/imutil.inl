#pragma once

#include <type_traits>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void edit_integer_field(const std::string& name, value_t& value)
    {
        int _int_value = static_cast<int>(value);
        ImGui::DragInt(name.c_str(), &_int_value);
        value = static_cast<value_t>(_int_value);
    }

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value)
    {
        (value);
        ImGui::Text(name.c_str());
    }

}
}
