#include <imgui.h>
#include <implot.h>
#include <implot_internal.h>
#include <iostream>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/overlay.fwd>

#define BUNGEEGUM_USE_OVERLAY_PROFILER_SIZE 2000

namespace bungeegum {
namespace detail {

    struct scrolling_buffer {
        scrolling_buffer() = delete;
        scrolling_buffer(const scrolling_buffer& other) = default;
        scrolling_buffer& operator=(const scrolling_buffer& other) = default;
        scrolling_buffer(scrolling_buffer&& other) = default;
        scrolling_buffer& operator=(scrolling_buffer&& other) = default;

        scrolling_buffer(const std::string& name, int max_size = BUNGEEGUM_USE_OVERLAY_PROFILER_SIZE)
            : _name(name)
            , _max_size(max_size)
        {
            _data.reserve(_max_size);
        }

        void emplace(const float x, const float y)
        {
            if (_data.size() < _max_size)
                _data.push_back(float2 { x, y });
            else {
                _data[_offset] = float2 { x, y };
                _offset = (_offset + 1) % _max_size;
            }
        }

        float back_value() const
        {
            if ((_data.size() < _max_size) || (_offset == 0))
                return _data.back().y;
            else {
                return _data[_offset - 1].y;
            }
        }

        const std::string& name() const
        {
            return _name;
        }

        const float* data_x() const
        {
            return &(_data[0].x);
        }

        const float* data_y() const
        {
            return &(_data[0].y);
        }

        bool empty() const
        {
            return _data.empty();
        }

        std::size_t offset() const
        {
            return _offset;
        }

        std::size_t size() const
        {
            return _data.size();
        }

    private:
        std::string _name = {};
        std::size_t _max_size = 0;
        std::size_t _offset = 0;
        std::vector<float2> _data = {};
    };

    static std::vector<scrolling_buffer> buffers = {};
    static std::unordered_map<std::string, std::size_t> buffer_names = {};
    static float delta_time = 0.f;
    static float max_value = 0.f;

    void setup_profiler_overlay()
    {
        frames_chronometer& _chronometer = global().backend.profiler_chronometer;
        _chronometer.on_new_task([&](const frames_chronometer_task& task) {
            buffer_names.emplace(task.name, buffers.size());
            buffers.emplace_back(scrolling_buffer(task.name));
        });
        _chronometer.on_new_frame_for_each_task([&](const frames_chronometer_task& task) {
            std::size_t _index = buffer_names.at(task.name);
            float _duration = static_cast<float>(task.duration.count());
            if (_index > 0) {
                _duration += buffers[_index - 1].back_value();
            }
            buffers[_index].emplace(delta_time, _duration);
            max_value = math::max(max_value, 1.1f * _duration);
        });
    }

    void draw_profiler_overlay()
    {
        delta_time += ImGui::GetIO().DeltaTime;
        ImGui::ShowDemoWindow();
        ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
        if (ImGui::Begin("profiler##__bungeegum_window_profiler_title__", NULL, ImGuiWindowFlags_NoCollapse)) {
            static float history = 10.0f;
            // ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
            static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
            // ImPlot::PushStyleColor(ImPlotCol_PlotBg, { 1.f, 1.f, 0.f, 1.f });
            ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
            if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, -1), ImPlotFlags_NoMenus)) {
                // ImPlot::SetupLegend(ImPlotLocation_NorthWest, ImPlotLegendFlags_NoButtons);
                ImPlot::SetupLegend(ImPlotLocation_NorthWest);
                ImPlot::SetupAxes(NULL, NULL, flags, flags);
                ImPlot::SetupAxisLimits(ImAxis_X1, delta_time - history, delta_time, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -0.1f, max_value);
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                for (int _k = static_cast<int>(buffers.size()) - 1; _k >= 0; --_k) {
                    if (!buffers[_k].empty()) {
                        ImPlot::PlotShaded(
                            buffers[_k].name().c_str(), // label id
                            buffers[_k].data_x(), // xs
                            buffers[_k].data_y(), //xy
                            static_cast<int>(buffers[_k].size()), // count
                            0.f, // yref
                            0, // flags
                            static_cast<int>(buffers[_k].offset()),
                            static_cast<int>(2 * sizeof(float)));
                    }
                }

                ImPlot::EndPlot();
            }
            ImPlot::PopStyleColor();
        }
        ImGui::End();
    }
}
}