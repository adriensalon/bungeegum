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

#include <staticgui/overlay/widgets/hierarchy.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        void draw_hierarchy(context_state& context)
        {
            ImGui::SetNextWindowSize({ 800, 250 });
            if (ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoMove)) {
                auto _black = ImVec4(0.f, 0.f, 0.f, 1.f);
                context.widgets.iterate_datas([&](detail::widget_data& _widget_data) {
                    unsigned int _depth = context.widgets.get_depth(_widget_data);
                    for (unsigned int _k = 0; _k < _depth; _k++) {
                        ImGui::TextColored(_black, "      ");
                        ImGui::SameLine();
                    }
                    ImGui::TextColored(_black, _widget_data.kind->name());
                    if (_widget_data.drawer) {
                        ImGui::SameLine();
                        ImGui::TextColored(_black, " [painter]");
                    }
                });
            }
            ImGui::End();
        }
    }
}
}