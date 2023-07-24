#pragma once

#include <type_traits>

#include <imgui.h>

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value)
    {
        (value);
        ImGui::Text(name.c_str());
    }

    template <>
    void edit_field<float1>(const std::string& name, float1& value)
    {
        ImGui::DragFloat(name.c_str(), &value);
    }

    template <>
    void edit_field<float2>(const std::string& name, float2& value)
    {
        float* _data = &(value.x);
        ImGui::DragFloat2(name.c_str(), _data);
    }

    template <>
    void edit_field<float3>(const std::string& name, float3& value)
    {
        float* _data = &(value.x);
        ImGui::DragFloat3(name.c_str(), _data);
    }

    template <>
    void edit_field<float4>(const std::string& name, float4& value)
    {
        float* _data = &(value.x);
        ImGui::DragFloat4(name.c_str(), _data);
    }

    template <>
    void edit_field<int1>(const std::string& name, int1& value)
    {
        ImGui::DragInt(name.c_str(), &value);
    }
}
}
