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

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_hierarchy_" + name + "__";
        }

        void draw_tree_widgets_tab()
        {
            const widgets_manager& _widgets_manager = global().widgets;
            std::size_t _tree_widgets_count = _widgets_manager.widgets.size(); // NON ALL WIDGETS
            std::string _title = "tree widgets (" + std::to_string(_tree_widgets_count) + ")";
            if (ImGui::BeginTabItem((_title + tag("tree_widgets_tab")).c_str())) {
                // ImVec2 _available_size = ImGui::GetContentRegionAvail();

                unsigned int _id = 0;
                std::function<void(const widget_update_data&)> _tf = [&](const widget_update_data& _widget_data) {
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
                };
                _tf(global().widgets.root_update_data());

                ImGui::EndTabItem();
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
                ImVec2 _available_size = ImGui::GetContentRegionAvail();
                for (const std::pair<const uintptr_t, event_update_data>& _event_data : _events_manager) {
                    std::string _clean_typenames = {};
                    for (const std::type_index& _type : _event_data.second.kinds) {
                        std::string _clean_typename = backend_manager::to_clean_typename(_type.name());
                        _clean_typenames += _clean_typename + " ";
                    }
                    ImGui::Text(("event <" + _clean_typenames + ">").c_str());
                }
                ImGui::EndTabItem();
            }
        }

        void draw_running_animations_tab()
        {
            const animations_manager& _animations_manager = global().animations;
            backend_manager& _backend_manager = global().backend;
            std::string _title = "running animations (" + std::to_string(_animations_manager.size()) + ")";
            if (ImGui::BeginTabItem((_title + tag("running_animations_tab")).c_str())) {
                ImVec2 _available_size = ImGui::GetContentRegionAvail();
                for (const std::pair<const uintptr_t, animation_update_data>& _animation_data : _animations_manager) {
                    std::string _clean_typename = backend_manager::to_clean_typename(_animation_data.second.kind->name());
                    if (ImGui::Selectable(("animation <" + _clean_typename + ">").c_str())) {
                        _backend_manager.inspector_selected = _animation_data.first;
                    }
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
        }
        ImGui::End();
    }
}
}
