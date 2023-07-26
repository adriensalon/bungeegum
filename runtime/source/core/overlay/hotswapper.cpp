#include <algorithm>
#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/backend/backend.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/dialog.hpp>
#include <bungeegum/glue/imguarded.fwd>

namespace bungeegum {
namespace detail {

    namespace {

        constexpr float multiline_text_input_height = 50.f;

        static std::string current_tab = {};
        static std::vector<bool> selected_items = {};
        static std::size_t selected_count = 0;

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_hotswapper_" + name + "__";
        }

        void draw_definition(const std::string& text)
        {
            ImGui::Text(text.c_str());
        }

        void draw_path(const std::filesystem::path& path)
        {
            std::string _pathname = path.generic_string();
            std::string _filename;
            if (std::filesystem::is_regular_file(path)) {
                _filename = path.filename().generic_string();
            } else {
                _filename = path.generic_string();
                if (_filename.back() == '/') {
                    _filename = _filename.substr(0, _filename.length() - 1);
                } else {
                    _pathname += "/";
                }
                _filename = std::filesystem::path(_filename).filename().generic_string() + "/";
            }
            ImGui::Text(_filename.c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip(_pathname.c_str());
            }
        }

        template <typename item_t>
        void draw_items_cards(const std::string& name, std::vector<item_t>& items, const float frame_height_without_padding, const float footer_height)
        {
            if (name != current_tab) {
                selected_items.clear();
                selected_count = 0;
                current_tab = name;
            }
            selected_items.resize(items.size());
            float _frame_height = frame_height_without_padding + 2.f * ImGui::GetStyle().FramePadding.y;
            // ImVec2 _outer_size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 2.f * frame_height_without_padding - 3.f * ImGui::GetStyle().FramePadding.y);
            ImVec2 _outer_size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - footer_height);

            static ImGuiTableFlags _table_flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
            if (ImGui::BeginTable(tag(name + "_table").c_str(), 1, _table_flags, _outer_size)) {

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                ImVec2 _cursor_position = ImGui::GetCursorPos();
                for (std::size_t _k = 0; _k < items.size(); _k++) {
                    ImGui::PushID(static_cast<int>(_k));
                    std::string _item_str;
                    if constexpr (std::is_same_v<item_t, std::string>) {
                        _item_str = items[_k];
                    } else {
                        _item_str = items[_k].generic_string();
                    }
                    ImGui::SetCursorPos(ImVec2(_cursor_position.x, _cursor_position.y));
                    ImGui::Selectable(("##" + _item_str).c_str(), false, ImGuiSelectableFlags_AllowOverlap, ImVec2(ImGui::GetContentRegionAvail().x, frame_height_without_padding));

                    ImGui::SetItemAllowOverlap();
                    if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
                        std::size_t _k_next = _k + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                        if (_k_next >= 0 && _k_next < items.size()) {
                            std::iter_swap(items.begin() + _k, items.begin() + _k_next);
                            ImGui::ResetMouseDragDelta();
                        }
                    }
                    _cursor_position.x += ImGui::GetStyle().FramePadding.x;
                    ImGui::SetCursorPos(_cursor_position);

                    //
                    //
                    //
                    //
                    bool vv = selected_items[_k];
                    if (ImGui::Checkbox(tag("cc" + _item_str).c_str(), &vv)) {
                        if (vv) {
                            selected_count++;
                        } else {
                            selected_count--;
                        }
                    }
                    selected_items[_k] = (vv);
                    _cursor_position.x -= ImGui::GetStyle().FramePadding.x;

                    ImGui::SameLine();

                    if constexpr (std::is_same_v<item_t, std::string>) {
                        draw_definition(items[_k]);
                    } else {
                        draw_path(items[_k]);
                    }
                    //
                    //
                    //
                    //

                    _cursor_position.y += _frame_height;
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }
        }

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

        template <typename item_t>
        void remove_selected(std::vector<item_t>& items)
        {
            for (int _k = static_cast<int>(items.size()) - 1; _k >= 0; --_k) {
                if (selected_items[_k]) {
                    items.erase(items.begin() + _k);
                }
            }
            selected_items.clear();
            selected_count = 0;
        }

        void draw_defines_buttons(std::vector<std::string>& defines)
        {
            static std::string _name_str = {};
            static std::string _value_str = {};
            float _available_width = ImGui::GetContentRegionAvail().x;
            ImGui::PushItemWidth(_available_width);
            ImGui::InputTextWithHint(tag("defines_name_input").c_str(), "type here a definition and below its value...", &_name_str);
            ImGui::PopItemWidth();
            ImGui::InputTextMultiline(tag("defines_value_input").c_str(), &_value_str, ImVec2(_available_width, multiline_text_input_height));
            float _button_width = 0.5f * (_available_width - ImGui::GetStyle().ItemSpacing.x);
            if (ImGui::Button("add definition", ImVec2(_button_width, 0.f))) {
                std::string _definition = _name_str;
                if (!_value_str.empty()) {
                    _definition += "=" + _value_str;
                }
                defines.push_back(_definition);
            }
            ImGui::SameLine();
            bool _disabled = selected_count == 0;
            if (_disabled) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("remove definitions", ImVec2(_button_width, 0.f))) {
                remove_selected<std::string>(defines);
            }
            if (_disabled) {
                ImGui::EndDisabled();
            }
        }

#if BUNGEEGUM_USE_DIALOG
        template <bool is_folder_t>
        void draw_paths_buttons(std::vector<std::filesystem::path>& paths, const std::vector<dialog_extensions_filter>& filters)
        {
            float _button_width = 0.5f * (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x);
            if constexpr (is_folder_t) {
                if (ImGui::Button("add folder...", ImVec2(_button_width, 0.f))) {
                    std::optional<std::filesystem::path> _picked_folder = pick_folder_dialog();
                    if (_picked_folder.has_value()) {
                        paths.push_back(_picked_folder.value());
                    }
                }
            } else {
                if (ImGui::Button("add file...", ImVec2(_button_width, 0.f))) {
                    std::optional<std::filesystem::path> _picked_file = open_file_dialog(filters);
                    if (_picked_file.has_value()) {
                        paths.push_back(_picked_file.value());
                    }
                }
            }
            ImGui::SameLine();
            bool _disabled = selected_count == 0;
            if (_disabled) {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button(is_folder_t ? "remove folders" : "remove files", ImVec2(_button_width, 0.f))) {
                remove_selected<std::filesystem::path>(paths);
            }
            if (_disabled) {
                ImGui::EndDisabled();
            }
        }
#endif

        void draw_hotswap_button()
        {
            color_guard _cg_button(ImGuiCol_Button, { 0.627f, 0.627f, 0.627f, 1.f });
            if (ImGui::Button("force update", ImVec2(ImGui::GetContentRegionAvail().x, 0.f))) {
                detail::global().backend.save_widgets("TODO"); // CREATE TEMP FILE HERE
                detail::global().backend.reload_manager->force_update();
                detail::global().backend.load_widgets("TODO"); // FIND TEMP FILE HERE AND DELETE
            }
        }

        void draw_defines(std::vector<std::string>& defines, const float frame_height_without_padding)
        {
            std::string _title = "defines (" + std::to_string(defines.size()) + ")" + tag("defines_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                ImGui::Spacing();
                float _footer_height = multiline_text_input_height + 3.f * ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
                draw_items_cards<std::string>("defines", defines, frame_height_without_padding, _footer_height);
                draw_defines_buttons(defines);
                ImGui::EndTabItem();
            }
        }

        template <bool is_folder_t>
        void draw_paths(const std::string& name, std::vector<std::filesystem::path>& paths, const float frame_height_without_padding, const std::vector<dialog_extensions_filter>& filters = {})
        {
            std::string _title = name + " (" + std::to_string(paths.size()) + ")" + tag(name + "_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                ImGui::Spacing();
                float _footer_height = 2.f * ImGui::GetFrameHeightWithSpacing();
                // float _footer_height = 2.f * frame_height_without_padding + 3.f * ImGui::GetStyle().FramePadding.y;
                draw_items_cards<std::filesystem::path>(name, paths, frame_height_without_padding, _footer_height);
#if BUNGEEGUM_USE_DIALOG
                draw_paths_buttons<is_folder_t>(paths, filters);
#else
                (void)filters;
#endif
                ImGui::EndTabItem();
            }
        }

        void draw_tabs(const float frame_height_without_padding = ImGui::GetFontSize())
        {
            if (ImGui::BeginTabBar(tag("tabs").c_str())) {
                draw_defines(defines(), frame_height_without_padding);
                draw_paths<true>("include dirs", include_directories(), frame_height_without_padding);
                draw_paths<true>("source dirs", source_directories(), frame_height_without_padding);
                draw_paths<false>("source files", force_compiled_source_files(), frame_height_without_padding, { { "Source File", { "c", "cpp", "cxx", "c++", "cc" } } });
                draw_paths<false>("libraries", libraries(), frame_height_without_padding, { { "Library", { "lib", "dll" } } });
            }
            ImGui::EndTabBar();
        }
    }

    void draw_hotswapper_overlay()
    {
#if BUNGEEGUM_USE_HOTSWAP
        ImGui::SetNextWindowSize({ 450.f, 550.f }, ImGuiCond_Once);
        if (ImGui::Begin("hotswapper##__bungeegum_window_hotswapper_title__", NULL, ImGuiWindowFlags_NoCollapse)) {

            color_guard _cg_button(ImGuiCol_Button, { 0.58f, 0.58f, 0.58f, 0.40f });
            // draw_tabs(30.f);
            draw_tabs(ImGui::GetFrameHeight());
            draw_hotswap_button();
        }
        ImGui::End();
#endif
    }
}
}
