
#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/backend/common.fwd>
#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/imguarded.fwd>

namespace bungeegum {
namespace detail {

    void draw_hotswapper_overlay()
    {
        ImGui::SetNextWindowSize({ 300.f, 450.f }, ImGuiCond_Once);
        if (ImGui::Begin("hotswapper##__bungeegum_window_hotswapper_title__", NULL, ImGuiWindowFlags_NoCollapse)) {

            // button ?
            if (ImGui::Button("Trigger hotswap")) {
                detail::global_manager::backend().reload_manager->force_update();
            }
        }
        ImGui::End();
    }
}
}
