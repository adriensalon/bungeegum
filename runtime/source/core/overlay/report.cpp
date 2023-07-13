#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>

namespace bungeegum {
namespace detail {

    void draw_report_overlay()
    {
        if (!global_manager::logs().userspace_errors.empty()) {
            ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
            // if (ImGui::Begin("report", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Modal)) {
            //     backtraced_exception& _exception = get_userspace_thrown_exception().value();
            //     ImGui::Text(_exception.what());
            //     ImGui::Spacing();
            //     ImGui::Separator();
            //     ImGui::Spacing();
            //     for (auto& _trace : _exception.tracing) {
            //         ImGui::TextColored(ImVec4(0.1f, 0.1f, 0.8f, 1.f), _trace.primary.function.c_str());
            //     }
            // }
            // ImGui::End();
        }
    }

}
}