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

        bool draw_inspected_widget(const std::uintptr_t raw_widget)
        {
            widgets_manager& _manager = global().widgets;
            if (_manager.contains(raw_widget)) {
                //
                //
                //
                return true;
            }
            return false;
        }

        bool draw_inspected_animation(const std::uintptr_t raw_animation)
        {
            animations_manager& _manager = global().animations;
            if (_manager.contains(raw_animation)) {
                //
                //
                //
                return true;
            }
            return false;
        }
    }

    void draw_inspector_overlay()
    {
        ImGui::SetNextWindowSize({ 300, 450 }, ImGuiCond_Once);
        if (ImGui::Begin(("inspector" + tag("window_title")).c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {
            backend_manager& _manager = global().backend;
            if (!_manager.inspector_selected.has_value()) {
                ImGui::Text("No widget selected.");
            } else {
                std::uintptr_t _raw_object = _manager.inspector_selected.value();
                if (!draw_inspected_widget(_raw_object)) {
                    draw_inspected_animation(_raw_object);
                }
            }
        }
        ImGui::End();
    }
}
}