#include <string>

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/overlay.fwd>

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_inspector_" + name + "__";
        }

    }

    void draw_inspector_overlay()
    {
        ImGui::SetNextWindowSize({ 300, 450 }, ImGuiCond_Once);
        if (ImGui::Begin(("inspector" + tag("window_title")).c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {
        }
        ImGui::End();
    }
}
}