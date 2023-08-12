#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <implot_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/glue/regex.hpp>

#define BUNGEEGUM_USE_OVERLAY_PROFILER_SIZE 2000

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_profiler_" + name + "__";
        }

        float backend_lifetime_duration()
        {
            return 0.001f * static_cast<float>(global().backend.lifetime_duration.count());
        }

        static std::string filter_text = "";
        static bool filter_enabled = true;

        template <typename value_t>
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

            void emplace(const value_t& value)
            {
                if (_data.size() < _max_size)
                    _data.push_back(value);
                else {
                    _data[_offset] = value;
                    _offset = (_offset + 1) % _max_size;
                }
            }

            void clear()
            {
                if constexpr (std::is_same_v<value_t, float2>) {
                    for (value_t& _point : _data) {
                        _point.y = 0.f;
                    }
                }
            }

            value_t average() const
            {
                value_t _average = {};
                for (const value_t& _point : _data) {
                    _average += _point;
                }
                _average /= _data.size();
                return _average;
            }

            value_t back() const
            {
                if ((_data.size() < _max_size) || (_offset == 0))
                    return _data.back();
                else {
                    return _data[_offset - 1];
                }
            }

            const std::string& name() const
            {
                return _name;
            }

            const value_t* data() const
            {
                return _data.data();
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

            bool visible = true;

        private:
            const std::string _name;
            const std::size_t _max_size;
            std::size_t _offset = 0;
            std::vector<value_t> _data = {};
        };

        struct scrolling_profiler {

            scrolling_profiler(const std::string& name)
                : _name(name)
            {
            }

            void setup(frames_chronometer& chronometer)
            {
                chronometer.on_new_task([&](const frames_chronometer_task& task) {
                    _buffer_names.emplace(task.name, _buffers.size());
                    _buffers.emplace_back(scrolling_buffer<float2>(task.name));
                });
                chronometer.on_new_frame_for_each_task([&](const frames_chronometer_task& task) {
                    float _lifetime_duration = backend_lifetime_duration();
                    std::size_t _index = _buffer_names.at(task.name);
                    if (_buffers[_index].visible) {
                        float _duration = static_cast<float>(task.duration.count());
                        if (_index > 0) {
                            _duration += _buffers[_index - 1].back().y;
                        }
                        _buffers[_index].emplace(float2 { _lifetime_duration, _duration });
                        _max_value = math::max(_max_value, 1.1f * _duration);
                    } else {
                        _buffers[_index].emplace(float2 { _lifetime_duration, 0.f });
                    }
                });
            }

            std::size_t update_average_frame_duration(const frames_chronometer& chronometer)
            {
                _average_frame_durations.emplace(chronometer.frame_duration().count());
                return _average_frame_durations.average();
            }

            void draw(const float history_size_seconds, const ImVec2 size = { -1.f, -1.f })
            {
                float _lifetime_duration = backend_lifetime_duration();
                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                if (ImPlot::BeginPlot(tag("scrolling_profiler_" + _name).c_str(), size, ImPlotFlags_NoMenus)) {
                    ImPlot::SetupLegend(ImPlotLocation_NorthWest, ImPlotLegendFlags_NoButtons);
                    ImPlot::SetupAxes(NULL, NULL, flags, flags);
                    ImPlot::SetupAxisLimits(ImAxis_X1, _lifetime_duration - history_size_seconds, _lifetime_duration, ImGuiCond_Always);
                    ImPlot::SetupAxisLimits(ImAxis_Y1, -0.1f, _max_value);
                    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                    for (int _k = static_cast<int>(_buffers.size()) - 1; _k >= 0; --_k) {
                        if (!_buffers[_k].empty()) {
                            _buffers[_k].visible = (!filter_enabled || (regex_search(_buffers[_k].name(), filter_text)));
                            if (_buffers[_k].visible) {
                                ImPlot::PlotShaded(
                                    _buffers[_k].name().c_str(), // label id
                                    &(_buffers[_k].data()->x), // xs
                                    &(_buffers[_k].data()->y), //xy
                                    static_cast<int>(_buffers[_k].size()), // count
                                    0.f, // yref
                                    0, // flags
                                    static_cast<int>(_buffers[_k].offset()),
                                    static_cast<int>(2 * sizeof(float)));
                            }
                        }
                    }

                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleColor();
            }

            void clear()
            {
                for (scrolling_buffer<float2>& _buffer : _buffers) {
                    _buffer.clear();
                }
            }

            const std::string& name() const
            {
                return _name;
            }

        private:
            const std::string _name;
            std::vector<scrolling_buffer<float2>> _buffers = {};
            std::unordered_map<std::string, std::size_t> _buffer_names = {};
            float _max_value = 0.f;
            scrolling_buffer<std::size_t> _average_frame_durations = { "frame_durations", 60 };
        };

        static scrolling_profiler frame = { "frame" };
        static scrolling_profiler resolve = { "resolve" };
        static scrolling_profiler interact = { "interact" };
        static scrolling_profiler draw = { "draw" };

        void draw_profiler_tab(frames_chronometer& chronometer, scrolling_profiler& profiler, const float history_size_seconds)
        {
            // SI TOUTES LES FRAMES
            const std::size_t _duration_ms = profiler.update_average_frame_duration(chronometer);
            // SINON
            // const std::size_t _duration_ms = chronometer.frame_duration().count();
            std::string _title = profiler.name() + " (" + std::to_string(_duration_ms) + "ms)" + tag(profiler.name() + "_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                ImVec2 _available_size = ImGui::GetContentRegionAvail();
                _available_size.y -= ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y;
                profiler.draw(history_size_seconds, _available_size);
                ImGui::EndTabItem();
            }
        }

        void draw_profiler_tab_with_regex(frames_chronometer& chronometer, scrolling_profiler& profiler, const float history_size_seconds)
        {
            // SI TOUTES LES FRAMES
            const std::size_t _duration_ms = profiler.update_average_frame_duration(chronometer);
            // SINON
            // const std::size_t _duration_ms = chronometer.frame_duration().count()
            std::string _title = profiler.name() + " (" + std::to_string(_duration_ms) + "ms)" + tag(profiler.name() + "_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                ImVec2 _available_size = ImGui::GetContentRegionAvail();
                _available_size.y -= 2.f * (ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y);
                profiler.draw(history_size_seconds, _available_size);
                if (ImGui::Checkbox(tag("filter_checkbox").c_str(), &filter_enabled)) {
                    if (!filter_text.empty()) {
                        profiler.clear();
                    }
                }
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::InputTextWithHint(tag("filter_input").c_str(), "type here a widget type to match with regex...", &filter_text)) {
                    if (filter_enabled) {
                        profiler.clear();
                    }
                }
                ImGui::PopItemWidth();
                ImGui::EndTabItem();
            }
        }
    }

    void setup_profiler_overlay()
    {
        frame.setup(global().backend.profiler_frame_chronometer);
        resolve.setup(global().backend.profiler_resolve_chronometer);
        interact.setup(global().backend.profiler_interact_chronometer);
        draw.setup(global().backend.profiler_draw_chronometer);
    }

    void draw_profiler_overlay()
    {
        ImGui::SetNextWindowSize({ 700, 350 }, ImGuiCond_Once);
        if (ImGui::Begin(("profiler" + tag("window_title")).c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {
            static float _history = 10.f;
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar(tag("tab_bar").c_str(), tab_bar_flags)) {
                draw_profiler_tab(global().backend.profiler_frame_chronometer, frame, _history);
                draw_profiler_tab_with_regex(global().backend.profiler_resolve_chronometer, resolve, _history);
                draw_profiler_tab_with_regex(global().backend.profiler_interact_chronometer, interact, _history);
                draw_profiler_tab_with_regex(global().backend.profiler_draw_chronometer, draw, _history);
            }
            ImGui::EndTabBar();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::DragFloat(tag("history_drag").c_str(), &_history, 0.1f, 1.f, BUNGEEGUM_USE_OVERLAY_PROFILER_SIZE / 100.f, "%.1f seconds");
            ImGui::PopItemWidth();
        }
        ImGui::End();
    }
}
}