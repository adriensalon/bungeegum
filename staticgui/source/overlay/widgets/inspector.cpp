//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <staticgui/overlay/widgets/inspector.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        void draw_inspector(context_state& context)
        {
            ImGui::SetNextWindowSize({ 800, 250 });
            if (ImGui::Begin("Inspector")) {

                ImGui::End();
            }
        }
    }
}
}