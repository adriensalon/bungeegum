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

#include <staticgui/overlay/widgets/inspector.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        void draw_inspector(context_state& context)
        {
            ImGui::SetNextWindowSize({ 800, 250 });
            if (ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoMove)) {
                int _k = 0;
                context.animations.iterate_datas([&](detail::animation_data& _animation_data) {
                    if (_animation_data.is_playing) {
                        std::string _title = "##StatsGraphTitle" + std::to_string(_k);
                        if (ImPlot::BeginPlot(_title.c_str(), { ImGui::GetContentRegionAvail().x, 150 })) {
                            auto _samples = _animation_data.curve.spline->get_strided_samples(100);
                            auto _point = _animation_data.t_curve;
                            ImPlot::PlotLine("", _samples.data(), &(_samples[1]), 100, 0, 0, 2 * sizeof(float));
                            ImPlot::PlotScatter("", _point.data(), _point.data() + 1, 1, 0, 0, 2 * sizeof(float));
                            ImPlot::EndPlot();
                        }
                    }
                    _k++;
                });
            }
            ImGui::End();
        }
    }
}
}