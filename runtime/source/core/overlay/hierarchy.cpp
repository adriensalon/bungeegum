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

        bool ends_with(std::string_view str, std::string_view suffix)
        {
            return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
        }

        bool starts_with(std::string_view str, std::string_view prefix)
        {
            return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
        }

        std::string size_to_string(const float2 size)
        {
            return (std::to_string(size.x) + ", " + std::to_string(size.y));
        }

        const std::string& clean_typename(const char* raw_typename)
        {
            static std::unordered_map<const char*, std::string> _typenames;
            if (_typenames.find(raw_typename) == _typenames.end()) {
                std::string _typename(raw_typename);
                std::regex _to_remove("struct bungeegum::widgets::");
                _typename = std::regex_replace(_typename, _to_remove, "");
                _typenames.emplace(raw_typename, _typename);
            }
            return _typenames[raw_typename];
        }
    }

    void draw_hierarchy_overlay()
    {
        ImGui::SetNextWindowSize({ 300.f, 450.f }, ImGuiCond_Once);
        if (ImGui::Begin("hierarchy##__bungeegum_window_hierarchy_title__", NULL, ImGuiWindowFlags_NoCollapse)) {
            unsigned int _depth = 0;
            unsigned int _id = 0;
            std::function<void(const widget_update_data&)> _tf = [&](const widget_update_data& _widget_data) {
                _id++;
                std::string _clean_typename = clean_typename(_widget_data.kind->name());
                std::string _clean_id_typename = _clean_typename + "###__hierarchy__" + std::to_string(_id);
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
