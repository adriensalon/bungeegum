#include <cctype>
#include <regex>
#include <string>
#include <unordered_map>

#include <imgui.h>
#include <imgui_memory_editor.h>
#include <implot.h>
#include <iostream>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/core/overlay.fwd>
#include <bungeegum/core/widget.hpp>
#include <bungeegum/glue/imguarded.fwd>
#include <bungeegum/glue/regex.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_hierarchy_" + name + "__";
        }

        static bool filter_enabled = true;
        static std::string filter_text = {};

        void draw_tree_widgets_tab()
        {
            const widgets_manager& _widgets_manager = global().widgets;
            std::size_t _tree_widgets_count = _widgets_manager.widgets.size(); // NON ALL WIDGETS
            std::string _title = "tree widgets (" + std::to_string(_tree_widgets_count) + ")";

            bool _tab_open = ImGui::BeginTabItem((_title + tag("tree_widgets_tab")).c_str());
            if (_tab_open) {
                // ImVec2 _available_size = ImGui::GetContentRegionAvail();

                ImVec2 _outer_size = ImVec2(0.0f, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().WindowPadding.y - ImGui::GetFrameHeight());
                ImGui::BeginChild(tag("child_tt").c_str(), _outer_size, false);

                unsigned int _id = 0;
                std::function<void(const widget_update_data&)> _tf = [&](const widget_update_data& _widget_data) {
                    if (!filter_enabled || (regex_search(_widget_data.clean_typename, filter_text))) {
                        _id++;
                        std::string _clean_id_typename = _widget_data.clean_typename + "###__hierarchy__" + std::to_string(_id);
                        // ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow ;
                        style_guard _indent_guard(ImGuiStyleVar_IndentSpacing, 10.f);
                        ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow;
                        if (_widget_data.children.empty())
                            _node_flags |= ImGuiTreeNodeFlags_Leaf;
                        font_guard _fg0(extrabold_font);

                        bool _tree_open = ImGui::TreeNodeEx(_clean_id_typename.c_str(), _node_flags);
                        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                            // // Do stuff on Selectable() double click.
                            // selected = !selected;
                            global().backend.inspector_selected = _widget_data.raw_widget;
                        }

                        if (_tree_open) {
                            _fg0.release();

                            // if (!_widget_data.children.empty())
                            // ImGui::Text((std::to_string(_widget_data.children.size()) + " children :").c_str());
                            for (auto& _child_widget_data_ref : _widget_data.children)
                                _tf(_child_widget_data_ref.get());
                            ImGui::TreePop();
                        }
                    } else {
                        for (auto& _child_widget_data_ref : _widget_data.children)
                            _tf(_child_widget_data_ref.get());
                    }
                };
                _tf(global().widgets.root_update_data());

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            if (_tab_open) {
                ImGui::Checkbox(tag("filter_checkbox").c_str(), &filter_enabled);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::InputTextWithHint(tag("filter_input").c_str(), "type here a widget type to match with regex...", &filter_text)) {
                    // todo
                }
                ImGui::PopItemWidth();
            }
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
            const events_manager& _events_manager = global().events;
            std::string _title = "async events (" + std::to_string(_events_manager.size()) + ")";
            if (ImGui::BeginTabItem((_title + tag("async_events_tab")).c_str())) {

                // GO ALGORITHM
                //
                std::size_t _args_count = 0;
                for (const std::pair<const uintptr_t, event_update_data>& _event_data : _events_manager) {
                    _args_count = std::max(_args_count, _event_data.second.clean_typenames.size());
                }
                //
                //

                static ImGuiTableFlags _table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;
                if (ImGui::BeginTable(tag("async_events_table").c_str(), static_cast<int>(_args_count + 1), _table_flags)) {

                    for (const std::pair<const uintptr_t, event_update_data>& _event_data : _events_manager) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("event");
                        const std::vector<std::string>& _clean_typenames = _event_data.second.clean_typenames;
                        int _index = 1;
                        for (const std::string& _clean_typename : _clean_typenames) {
                            ImGui::TableSetColumnIndex(_index);
                            ImGui::Text(_clean_typename.c_str());
                            _index++;
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }
        }

        void draw_running_animations_tab()
        {
            const animations_manager& _animations_manager = global().animations;
            // backend_manager& _backend_manager = global().backend;
            std::string _title = "running animations (" + std::to_string(_animations_manager.size()) + ")";
            if (ImGui::BeginTabItem((_title + tag("running_animations_tab")).c_str())) {
                static ImGuiTableFlags _table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;
                if (ImGui::BeginTable(tag("async_events_table").c_str(), 2, _table_flags)) {
                    for (const std::pair<const uintptr_t, animation_update_data>& _animation_data : _animations_manager) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        bool _selected = false;
                        ImGui::Selectable("animation", &_selected, ImGuiSelectableFlags_SpanAllColumns);
                        if (ImGui::IsItemHovered()) {
                            if (ImGui::BeginTooltip()) {
                                static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoTickMarks;
                                ImPlot::PushStyleColor(ImPlotCol_FrameBg, { 1.f, 1.f, 0.f, 0.f });
                                if (ImPlot::BeginPlot(tag("animation_graph").c_str(), { ImGui::GetContentRegionAvail().x, 90 }, flags)) {
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
                                ImGui::EndTooltip();
                            }
                        }
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text(_animation_data.second.clean_typename.c_str());
                    }
                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }
        }
    }

    void draw_hierarchy_overlay()
    {
        ImGui::SetNextWindowSize({ 380.f, 550.f }, ImGuiCond_Once);
        if (ImGui::Begin(("hierarchy" + tag("window_title")).c_str(), NULL, ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar(tag("tab_bar").c_str())) {
                draw_tree_widgets_tab();
                draw_orphan_widgets_tab();
                draw_async_events_tab();
                draw_running_animations_tab();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
}
