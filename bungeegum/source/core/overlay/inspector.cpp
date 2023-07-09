#include <string>

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/core/overlay.fwd>

namespace bungeegum {
namespace detail {

    void draw_inspector_overlay()
    {
        ImGui::SetNextWindowSize({ 300, 450 }, ImGuiCond_Once);
        if (ImGui::Begin("inspector##__bungeegum_window_inspector_title__", 0, ImGuiWindowFlags_NoCollapse)) {
            int _k = 0;
            for (const auto& _event_data : global_events_manager) {
                (void)_event_data;
                ImGui::Text("event future....");
            }
            for (const auto& _animation_data : global_animations_manager) {
                // if (_animation_data.is_playing) {
                std::string _title = "##StatsGraphTitle" + std::to_string(_k);
                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                if (ImPlot::BeginPlot(_title.c_str(), { ImGui::GetContentRegionAvail().x, 90 }, flags)) {
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                    // auto _samples = _animation_data.eval_curve.strided_samples(100);
                    // auto _point = std::vector<float> { _animation_data.eval_point.x, _animation_data.eval_point.y };
                    auto _samples = _animation_data.second.overlay_samples;
                    auto _point = _animation_data.second.overlay_position;
                    ImPlot::PlotLine("", _samples.data(), &(_samples[1]), 100, 0, 0, 2 * sizeof(float));
                    ImPlot::PlotScatter("", _point.data(), _point.data() + 1, 1, 0, 0, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleColor();
                // }
                _k++;
            }
        }
        ImGui::End();
    }
}
}