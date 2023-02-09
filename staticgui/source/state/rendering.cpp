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

#include <iostream>

#include <staticgui/state/rendering.hpp>

namespace staticgui {
namespace detail {

    void command_data::add_command(const std::function<void(ImDrawList*)>& command_callback)
    {
        _commands.emplace_back(command_callback);
    }

    void command_data::draw()
    {
        ImDrawList* _draw_list = ImGui::GetBackgroundDrawList();
        draw(_draw_list);
    }

    void command_data::draw(ImDrawList* imgui_drawlist)
    {
        for (auto& _command : _commands)
            _command(imgui_drawlist);
    }

    void command_data::clear()
    {
        _commands.clear();
    }

}
}