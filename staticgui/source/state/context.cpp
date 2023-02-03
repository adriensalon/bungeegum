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

#include <staticgui/state/context.hpp>

namespace staticgui {
namespace detail {

    bool context_state::tick(const float delta_milliseconds)
    {
        animations.tick(delta_milliseconds);
        events.tick();

        // parse tree et return true si besoin de refresh
        return true;
    }

    void context_state::draw()
    {
        ImGui::ShowDemoWindow();
        // iterate widgets and draw OU draw direct (encapsulation) ??
    }
}
}