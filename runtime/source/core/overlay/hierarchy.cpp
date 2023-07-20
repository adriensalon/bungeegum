#include <cctype>
#include <regex>
#include <string>
#include <unordered_map>

#include <imgui.h>
#include <imgui_memory_editor.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/imguarded.fwd>

template <typename T>
std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << i;
    return stream.str();
}

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_hierarchy_" + name + "__";
        }

        std::string size_to_string(const float2 size)
        {
            return (std::to_string(size.x) + ", " + std::to_string(size.y));
        }

        void draw_tree_widgets_tab()
        {
            // std::size_t _tree_widgets_count;
            // std::string _title = "tree widgets (" + std::to_string(_tree_widgets_count) + ")";
            // if (ImGui::BeginTabItem((_title + tag("tree_widgets_tab")).c_str())) {
            //     ImVec2 _available_size = ImGui::GetContentRegionAvail();
            //     //
            //     //
            //     //
            //     //
            //     ImGui::EndTabItem();
            // }
        }

        void draw_orphan_widgets_tab()
        {
            // std::size_t _orphan_widgets_count;
            // std::string _title = "orphan widgets (" + std::to_string(_orphan_widgets_count) + ")";
            // if (ImGui::BeginTabItem((_title + tag("orphan_widgets_tab")).c_str())) {
            //     ImVec2 _available_size = ImGui::GetContentRegionAvail();
            //     //
            //     //
            //     //
            //     //
            //     ImGui::EndTabItem();
            // }
        }

        void draw_async_events_tab()
        {
            const events_manager& _manager = global().events;
            std::string _title = "async events (" + std::to_string(_manager.size()) + ")";
            if (ImGui::BeginTabItem((_title + tag("async_events_tab")).c_str())) {
                ImVec2 _available_size = ImGui::GetContentRegionAvail();
                for (const std::pair<const uintptr_t, event_update_data>& _event_data : _manager) {
                    (void)_event_data;
                    ImGui::Text("event future....");
                }
                ImGui::EndTabItem();
            }
        }

        void draw_running_animations_tab()
        {
            animations_manager& _manager = global().animations;
            std::string _title = "running animations (" + std::to_string(_manager.size()) + ")";
            if (ImGui::BeginTabItem((_title + tag("running_animations_tab")).c_str())) {
                ImVec2 _available_size = ImGui::GetContentRegionAvail();
                int _k = 0;
                for (const auto& _animation_data : global().animations) {
                    // if (_animation_data.is_playing) {
                    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                    ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                    if (ImPlot::BeginPlot(("##StatsGraphTitle" + std::to_string(_k)).c_str(), { ImGui::GetContentRegionAvail().x, 90 }, flags)) {
                        ImPlot::SetupAxes(NULL, NULL, flags, flags);
                        ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1, ImGuiCond_Always);
                        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
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
                ImGui::EndTabItem();
            }
        }
    }

    void draw_hierarchy_overlay()
    {
        ImGui::SetNextWindowSize({ 300.f, 450.f }, ImGuiCond_Once);
        if (ImGui::Begin(("hierarchy" + tag("window_title")).c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {

            if (ImGui::BeginTabBar(tag("tab_bar").c_str())) {
                draw_tree_widgets_tab();
                draw_orphan_widgets_tab();
                draw_async_events_tab();
                draw_running_animations_tab();
            }
            ImGui::EndTabBar();

            //
            //
            //
            //
            //
            //
            //
            //
            //
            //
            //
            //
            //

            unsigned int _depth = 0;
            unsigned int _id = 0;
            std::function<void(const widget_update_data&)> _tf = [&](const widget_update_data& _widget_data) {
                _id++;
                std::string _clean_id_typename = _widget_data.clean_typename + "###__hierarchy__" + std::to_string(_id);
                // ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow ;
                ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed;
                if (_widget_data.children.empty())
                    _node_flags |= ImGuiTreeNodeFlags_Leaf;
                font_guard _fg0(extrabold_font);
				
                if (ImGui::TreeNodeEx(_clean_id_typename.c_str(), _node_flags)) {
                    _fg0.release();
                    // if (_widget_data.resolver)
                    //     ImGui::Text("has advanced resolve");
                    // if (_widget_data.drawer)
                    //     ImGui::Text("has advanced draw");
                    if (ImGui::Button("print")) {
                        global().backend.inspect_reloadable_widget(_widget_data);
                    }
                    ImGui::Text(("sizeof =" + std::to_string(_widget_data.true_sizeof())).c_str());
                    ImGui::Text(("raw =" + int_to_hex(_widget_data.true_ptr())).c_str());

                    static MemoryEditor mem_edit_2;
                    mem_edit_2.DrawContents((void*)_widget_data.true_ptr(), _widget_data.true_sizeof(), _widget_data.true_ptr());

                    ImGui::Text("Constraints");
                    ImGui::Text(("min_size = " + size_to_string(_widget_data.resolver_command.min_size())).c_str());
                    ImGui::Text(("max_size = " + size_to_string(_widget_data.resolver_command.max_size())).c_str());

                    // ImGui::Text((std::to_string(_widget_data.command.value()._commands.size()) + " advanced draw commands)").c_str());
                    // if (!_widget_data.detached_events_removers.empty())
                    //     ImGui::Text((std::to_string(_widget_data.detached_events_removers.size()) + " detached events").c_str());
                    if (!_widget_data.children.empty())
                        ImGui::Text((std::to_string(_widget_data.children.size()) + " children :").c_str());
                    _depth++;
                    for (auto& _child_widget_data_ref : _widget_data.children)
                        _tf(_child_widget_data_ref.get());
                    ImGui::TreePop();
                }
            };

            _tf(global().widgets.root_update_data());
        }
        ImGui::End();
    }
}
}
