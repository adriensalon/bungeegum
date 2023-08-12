#pragma once

#include <type_traits>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/glue/simd.hpp>

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void edit_integer_field(const std::string& name, value_t& value)
    {
        int1 _int_value = static_cast<int1>(value);
        ImGui::DragInt(name.c_str(), &_int_value);
        value = static_cast<value_t>(_int_value);
    }

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value)
    {
        (value);
        ImGui::Text(name.c_str());
    }

    template <>
    void edit_field<std::string>(const std::string& name, std::string& value)
    {
        ImGui::InputText(name.c_str(), &value);
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
        edit_integer_field<int1>(name, value);
    }

    template <>
    void edit_field<uint1>(const std::string& name, uint1& value)
    {
        edit_integer_field<uint1>(name, value);
    }

    template <>
    void edit_field<std::int64_t>(const std::string& name, std::int64_t& value)
    {
        edit_integer_field<std::int64_t>(name, value);
    }

    template <>
    void edit_field<std::uint64_t>(const std::string& name, std::uint64_t& value)
    {
        edit_integer_field<std::uint64_t>(name, value);
    }
}
}
