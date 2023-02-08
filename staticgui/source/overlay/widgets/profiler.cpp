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

        void draw_profiler(context_state& context)
        {

            ImGui::SetNextWindowSize({ 800, 250 });
            if (ImGui::Begin("Profiler")) {

                static ScrollingBuffer sdata1, sdata2;
                ImVec2 mouse = ImGui::GetMousePos();
                static float t = 0;
                t += ImGui::GetIO().DeltaTime;
                sdata1.AddPoint(t, mouse.x * 0.0005f);
                sdata2.AddPoint(t, mouse.y * 0.0005f);

                static float history = 10.0f;
                ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");

                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

                if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                    ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, 0, sdata1.Offset, 2 * sizeof(float));
                    ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
                    ImPlot::EndPlot();
                }
                ImGui::End();
            }
        }
    }
}
}