#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/context.fwd>
#include <bungeegum/core/exceptions.hpp>

namespace bungeegum {
namespace detail {

    void draw_report_overlay()
    {
        if (has_userspace_thrown()) {
            ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
            if (ImGui::Begin("report", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Modal)) {
                backtraced_exception& _exception = get_userspace_thrown_exception().value();
                ImGui::Text(_exception.what());
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                for (auto& _trace : _exception.tracing) {
                    ImGui::TextColored(ImVec4(0.1f, 0.1f, 0.8f, 1.f), _trace.primary.function.c_str());
                }
            }
            ImGui::End();
        }
    }

}
}