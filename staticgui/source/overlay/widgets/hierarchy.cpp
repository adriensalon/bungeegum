//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <string>
#include <unordered_map>

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <staticgui/overlay/widgets/hierarchy.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        bool ends_with(std::string_view str, std::string_view suffix)
        {
            return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
        }

        bool starts_with(std::string_view str, std::string_view prefix)
        {
            return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
        }

        const std::string& clean_typename(const char* raw_typename)
        {
            static std::unordered_map<const char*, std::string> _typenames;
            if (_typenames.find(raw_typename) == _typenames.end()) {
                std::string _typename(raw_typename);
                if (starts_with(_typename, "class "))
                    _typename = _typename.substr(5, _typename.length() - 5);
                if (starts_with(_typename, "struct "))
                    _typename = _typename.substr(6, _typename.length() - 6);
                size_t _last_colon_pos = _typename.find_last_of(':');
                if (_last_colon_pos != std::string::npos)
                    _typename = _typename.substr(_last_colon_pos + 1, _typename.length() - _last_colon_pos - 1);
                size_t _last_underscore_pos = _typename.find_last_not_of('_');
                if (ends_with(_typename, "_widget"))
                    _typename = _typename.substr(0, _typename.length() - 7);
                _typenames.emplace(raw_typename, _typename);
            }
            return _typenames[raw_typename];
        }

        void draw_hierarchy(context_state& context)
        {
            if (ImGui::Begin("Hierarchy")) {
                unsigned int _depth = 0;
                unsigned int _id = 0;
                std::function<void(const widget_data&)> _tf = [&](const widget_data& _widget_data) {
                    _id++;
                    std::string _clean_typename = clean_typename(_widget_data.kind->name());
                    std::string _clean_id_typename = _clean_typename + "###__hierarchy__" + std::to_string(_id);
                    ImGuiTreeNodeFlags _node_flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow;
                    if (_widget_data.children.empty())
                        _node_flags |= ImGuiTreeNodeFlags_Leaf;
                    if (ImGui::TreeNodeEx(_clean_id_typename.c_str(), _node_flags)) {
                        if (_widget_data.resolver)
                            ImGui::Text("has advanced resolve");
                        if (_widget_data.drawer)
                            ImGui::Text("has advanced draw");
                        // ImGui::Text((std::to_string(_widget_data.command.value()._commands.size()) + " advanced draw commands)").c_str());
                        if (!_widget_data.detached_events_removers.empty())
                            ImGui::Text((std::to_string(_widget_data.detached_events_removers.size()) + " detached events").c_str());
                        if (!_widget_data.children.empty())
                            ImGui::Text((std::to_string(_widget_data.children.size()) + " children :").c_str());
                        _depth++;
                        for (auto& _child_widget_data_ref : _widget_data.children)
                            _tf(_child_widget_data_ref.get());
                        ImGui::TreePop();
                    }
                };
                context.widgets.iterate_root_datas(_tf);
            }
            ImGui::End();
        }
    }
}
}