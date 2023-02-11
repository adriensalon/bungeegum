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

#include <staticgui/overlay/widgets/report.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        void draw_report(context_state& context)
        {
            if (has_userspace_thrown()) {
                ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
                // ImGui::SetNextWindowBgAlpha(0.f);
                if (ImGui::Begin("report", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Modal)) {
                    // if () {
                    backtraced_exception& _exception = get_userspace_thrown_exception().value();
                    ImGui::Text(_exception.what());
                    ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::Spacing();
                    for (auto& _trace : _exception.tracing) {
                        ImGui::TextColored(ImVec4(0.1f, 0.1f, 0.8f, 1.f), _trace.primary.function.c_str());
                    }
                    // ImGui::Spacing();
                    // ImGui::Separator();
                    // ImGui::Spacing();
                    // }
                }
                ImGui::End();
            }
        }
    }
}
}