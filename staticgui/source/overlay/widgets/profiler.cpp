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

        namespace {
            class profiler {
            public:
                std::vector<ImVec4> colors = {
                    ImVec4(0.62f, 0.00f, 0.74f, 1.00f),
                    ImVec4(0.70f, 0.f, 0.f, 1.f),
                    ImVec4(0.22f, 0.f, 0.70f, 1.f),
                    ImVec4(0.f, 0.22f, 0.40f, 1.f),
                    ImVec4(0.120f, 0.22f, 0.22f, 1.f),
                };

                profiler(const unsigned int buffer_size = 1, const unsigned int max_frames_record_count = 300)
                    : _buffer_size(buffer_size)
                    , _max_frames_record_count(max_frames_record_count)
                {
                }

                // void new_frame(const std::vector<glite::timer::task>& timer_tasks);

                void render(const unsigned int max_tasks, const float bar_width = 3.f, const float max_height = 800.f, const float legend_width = 200.f);

            private:
                struct frame {
                    std::vector<unsigned int> indices = {};
                    std::vector<float> ratios = {};
                };

                struct task {
                    double duration = 0.0;
                    unsigned int count = 0;
                };

                const unsigned int _buffer_size;
                const unsigned int _max_frames_record_count;
                std::unordered_map<std::string, unsigned int> _task_names;
                std::vector<task> _tasks;
                std::vector<ImVec4> _colors;
                unsigned int _color_index = 0;
                std::vector<frame> _frames;
                std::unordered_map<unsigned int, std::vector<float>> _buffer;
                unsigned int _buffer_index = 0;

                void _render_bars(ImDrawList* draw_list, const float position_x, const float position_y, const float size_x, const float size_y, const float bar_width);

                void _render_legend(ImDrawList* draw_list, const float position_x, const float position_y, const float size_x, const float size_y);
            };
        }

        // utility structure for realtime plot
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
        static std::vector<unsigned int> _indices;
        static std::vector<ScrollingBuffer> _buffers;
        static float _delta_time = 0.f;
        static float _max = 0.f;

        void setup_profiler(context_state& context)
        {
            context.frames_chronometer.on_new_task([&](const std::string& _name, const unsigned int _index) {
                _count++;
                _names.push_back(_name);
                _indices.push_back(_index);
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
            ImGui::SetNextWindowSize({ 800, 250 });
            if (ImGui::Begin("Profiler")) {

                static float history = 10.0f;
                ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

                if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, _delta_time - history, _delta_time, ImGuiCond_Always);

                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, _max);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

                    for (unsigned int _k = 0; _k < _count; _k++)
                        ImPlot::PlotLine(_names[_k].c_str(), &(_buffers[_k].Data[0].x), &(_buffers[_k].Data[0].y), _buffers[_k].Data.size(), 0, _buffers[_k].Offset, 2 * sizeof(float));

                    // ImPlot::PlotShaded(_names[0].c_str(), &(_buffers[0].Data[0].x), &(_buffers[0].Data[0].y), _buffers[0].Data.size(), -INFINITY, 0, _buffers[0].Offset, 2 * sizeof(float));
                    // for (unsigned int _k = 1; _k < _count; _k++)
                    //     ImPlot::PlotShaded(_names[_k].c_str(), &(_buffers[_k].Data[0].x), &(_buffers[_k].Data[0].y), _buffers[_k].Data.size(), _buffers[_k - 1].Data[0].y, 0, _buffers[_k].Offset, 2 * sizeof(float));

                    ImPlot::EndPlot();
                }
                ImGui::End();
            }
        }
    }
}
}