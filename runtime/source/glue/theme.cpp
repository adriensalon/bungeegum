
#include <bungeegum/glue/theme.hpp>

#include <imgui.h>
#include <imgui_internal.h>

namespace bungeegum {
namespace detail {

#pragma region font_guard
    font_guard::font_guard()
    {
    }

    font_guard::font_guard(ImFont* font)
    {
        set(font);
    }

    font_guard::~font_guard()
    {
        release();
    }

    void font_guard::set(ImFont* font)
    {
        if (!_is_set)
            ImGui::PushFont(font);
        _is_set = true;
    }

    void font_guard::release()
    {
        if (_is_set)
            ImGui::PopFont();
        _is_set = false;
    }
#pragma endregion

#pragma region item_guard
    item_guard::item_guard()
    {
    }

    item_guard::item_guard(const int target, const bool value)
    {
        set(target, value);
    }

    item_guard::~item_guard()
    {
        release();
    }

    void item_guard::set(const int target, const bool value)
    {
        if (!_is_set)
            ImGui::PushItemFlag(target, value);
        _is_set = true;
    }

    void item_guard::release()
    {
        if (_is_set)
            ImGui::PopItemFlag();
        _is_set = false;
    }
#pragma endregion

#pragma region color_guard
    color_guard::color_guard()
    {
    }

    color_guard::color_guard(const int& target, const float4 value)
    {
        set(target, value);
    }

    color_guard::~color_guard()
    {
        release();
    }

    void color_guard::set(const int& target, const float4 value)
    {
        if (!_is_set)
            ImGui::PushStyleColor(target, ImVec4(value.x, value.y, value.z, value.w));
        _is_set = true;
    }

    void color_guard::release()
    {
        if (_is_set)
            ImGui::PopStyleColor();
        _is_set = false;
    }
#pragma endregion

#pragma region style_guard
    style_guard::style_guard()
    {
    }

    style_guard::style_guard(const int& target, const float value)
    {
        set(target, value);
    }

    style_guard::style_guard(const int& target, const float2 value)
    {
        set(target, value);
    }

    style_guard::~style_guard()
    {
        release();
    }

    void style_guard::set(const int& target, const float value)
    {
        if (!_is_set)
            ImGui::PushStyleVar(target, value);
        _is_set = true;
    }

    void style_guard::set(const int& target, const float2 value)
    {
        if (!_is_set)
            ImGui::PushStyleVar(target, ImVec2(value.x, value.y));
        _is_set = true;
    }

    void style_guard::release()
    {
        if (_is_set)
            ImGui::PopStyleVar();
        _is_set = false;
    }
#pragma endregion

    template <>
    void edit_field<std::string>(const std::string& name, std::string& value)
    {
        ImGui::InputText(name.c_str(), &value);
    }

    template <>
    void edit_field<bool>(const std::string& name, bool& value)
    {
        ImGui::Checkbox(name.c_str(), &value);
    }

    template <>
    void edit_field<float>(const std::string& name, float& value)
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
    void edit_field<int>(const std::string& name, int& value)
    {
        edit_integer_field<int>(name, value);
    }

    template <>
    void edit_field<unsigned int>(const std::string& name, unsigned int& value)
    {
        edit_integer_field<unsigned int>(name, value);
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
