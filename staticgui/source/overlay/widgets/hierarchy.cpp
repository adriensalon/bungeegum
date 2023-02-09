//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

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
            // ImGui::SetNextWindowSize({ 800, 250 });
            // if (ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoMove)) {
            //     auto _black = ImVec4(0.f, 0.f, 0.f, 1.f);
            //     context.widgets.iterate_datas([&](detail::widget_data& _widget_data) {
            //         unsigned int _depth = context.widgets.get_depth(_widget_data);
            //         for (unsigned int _k = 0; _k < _depth; _k++) {
            //             ImGui::TextColored(_black, "      ");
            //             ImGui::SameLine();
            //         }
            //         ImGui::TextColored(_black, clean_typename(_widget_data.kind->name()).c_str());
            //         if (_widget_data.drawer) {
            //             ImGui::SameLine();
            //             ImGui::TextColored(_black, " [painter]");
            //         }
            //     });
            // }
            // ImGui::End();

            if (ImGui::Begin("Hierarchy")) {
                context.widgets.iterate_datas([&](detail::widget_data& _widget_data) {
                    unsigned int _depth = context.widgets.get_depth(_widget_data);
                    for (unsigned int _k = 0; _k < _depth; _k++) {
                        ImGui::Text("      ");
                        ImGui::SameLine();
                    }
                    ImGui::Text(clean_typename(_widget_data.kind->name()).c_str());
                });
            }
            ImGui::End();
        }
    }
}
}