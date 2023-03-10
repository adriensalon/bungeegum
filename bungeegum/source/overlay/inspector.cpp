//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <string>

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/animation.hpp>
#include <bungeegum/overlay/inspector.hpp>

namespace bungeegum {
namespace detail {

    void draw_inspector_overlay()
    {
        ImGui::SetNextWindowSize({ 300, 450 }, ImGuiCond_Once);
        if (ImGui::Begin("inspector##__bungeegum_window_inspector_title__", 0, ImGuiWindowFlags_NoCollapse)) {
            int _k = 0;
            animations_context.iterate([&](untyped_animation_data& _animation_data) {
                // if (_animation_data.is_playing) {
                std::string _title = "##StatsGraphTitle" + std::to_string(_k);
                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                if (ImPlot::BeginPlot(_title.c_str(), { ImGui::GetContentRegionAvail().x, 90 }, flags)) {
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                    auto _samples = _animation_data.eval_curve.strided_samples(100);
                    auto _point = std::vector<float> { _animation_data.eval_point.x, _animation_data.eval_point.y };
                    ImPlot::PlotLine("", _samples.data(), &(_samples[1]), 100, 0, 0, 2 * sizeof(float));
                    ImPlot::PlotScatter("", _point.data(), _point.data() + 1, 1, 0, 0, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleColor();
                // }
                _k++;
            });
        }
        ImGui::End();
    }
}
}