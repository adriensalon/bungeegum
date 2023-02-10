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

#include <staticgui/overlay/widgets/profiler.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        struct ScrollingBuffer {
            int MaxSize;
            int Offset;
            ImVector<ImVec2> Data;
            ScrollingBuffer(int max_size = 2000)
            {
                MaxSize = max_size;
                Offset = 0;
                Data.reserve(MaxSize);
            }
            void AddPoint(float x, float y)
            {
                if (Data.size() < MaxSize)
                    Data.push_back(ImVec2(x, y));
                else {
                    Data[Offset] = ImVec2(x, y);
                    Offset = (Offset + 1) % MaxSize;
                }
            }
            void Erase()
            {
                if (Data.size() > 0) {
                    Data.shrink(0);
                    Offset = 0;
                }
            }
        };

        static unsigned int _count = 0;
        static std::vector<std::string> _names;
        // static std::vector<unsigned int> _indices;
        static std::vector<ScrollingBuffer> _buffers;
        static float _delta_time = 0.f;
        static float _max = 0.f;

        void setup_profiler(context_state& context)
        {
            context.frames_chronometer.on_new_task([&](const std::string& _name, const unsigned int _index) {
                _count++;
                _names.push_back(_name);
                // _indices.push_back(_index);
                _buffers.emplace_back(ScrollingBuffer());
            });
            context.frames_chronometer.on_new_frame([&]() {
                _delta_time += ImGui::GetIO().DeltaTime;
                for (unsigned int _k = 0; _k < _count; _k++) {
                    float _ratio = context.frames_chronometer.get_frames().back().ratios[_k];
                    _buffers[_k].AddPoint(_delta_time, _ratio);
                    _max = std::fmax(_max, 1.1f * _ratio);
                }
            });
        }

        void draw_profiler(context_state& context)
        {
            ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
            if (ImGui::Begin("profiler##__staticgui_window_profiler_title__", NULL, ImGuiWindowFlags_NoCollapse)) {
                static float history = 10.0f;
                // ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                // ImPlot::PushStyleColor(ImPlotCol_PlotBg, { 1.f, 1.f, 0.f, 1.f });
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, -1))) {
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, _delta_time - history, _delta_time, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -0.1f, _max);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                    for (unsigned int _k = 0; _k < _count; _k++)
                        ImPlot::PlotLine(_names[_k].c_str(), &(_buffers[_k].Data[0].x), &(_buffers[_k].Data[0].y), _buffers[_k].Data.size(), 0, _buffers[_k].Offset, 2 * sizeof(float));

                    // ImPlot::PlotShaded(_names[0].c_str(), &(_buffers[0].Data[0].x), &(_buffers[0].Data[0].y), _buffers[0].Data.size(), -INFINITY, 0, _buffers[0].Offset, 2 * sizeof(float));
                    // for (unsigned int _k = 1; _k < _count; _k++)
                    //     ImPlot::PlotShaded(_names[_k].c_str(), &(_buffers[_k].Data[0].x), &(_buffers[_k].Data[0].y), _buffers[_k].Data.size(), _buffers[_k - 1].Data[0].y, 0, _buffers[_k].Offset, 2 * sizeof(float));

                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleColor();
            }
            ImGui::End();
        }
    }
}
}