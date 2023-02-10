//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <staticgui/glue/imguarded.hpp>

namespace staticgui {
namespace glue {

    font_guard::font_guard() { }

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

    item_guard::item_guard() { }

    item_guard::item_guard(const int target, const bool value)
    {
        set(target, value);
    }

    item_guard::~item_guard()
    {
        if (_is_set)
            ImGui::PopItemFlag();
    }

    void item_guard::set(const int target, const bool value)
    {
        if (_is_set)
            ImGui::PopItemFlag();
        else
            _is_set = true;
        ImGui::PushItemFlag(target, value);
    }

    color_guard::color_guard() { }

    color_guard::color_guard(const int& target, const simd_array<float, 4>& value)
    {
        set(target, value);
    }

    color_guard::~color_guard()
    {
        if (_is_set)
            ImGui::PopStyleColor();
    }

    void color_guard::set(const int& target, const simd_array<float, 4>& value)
    {
        if (_is_set)
            ImGui::PopStyleColor();
        else
            _is_set = true;
        ImGui::PushStyleColor(target, ImVec4(value.x(), value.y(), value.z(), value.w()));
    }

    style_guard::style_guard() { }

    style_guard::style_guard(const int& target, const float value)
    {
        set(target, value);
    }

    style_guard::style_guard(const int& target, const simd_array<float, 2>& value)
    {
        set(target, value);
    }

    style_guard::~style_guard()
    {
        if (_is_set && !_is_released)
            ImGui::PopStyleVar();
    }

    void style_guard::set(const int& target, const float value)
    {
        if (_is_set)
            ImGui::PopStyleVar();
        else
            _is_set = true;
        ImGui::PushStyleVar(target, value);
    }

    void style_guard::set(const int& target, const simd_array<float, 2>& value)
    {
        ImGui::PushStyleVar(target, ImVec2(value.x(), value.y()));
        _is_set = true;
    }

    void style_guard::release()
    {
        if (_is_set)
            ImGui::PopStyleVar();
        _is_released = true;
    }

}
}
