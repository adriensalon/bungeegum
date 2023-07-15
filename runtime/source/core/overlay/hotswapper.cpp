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

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_hotswapper_" + name + "__";
        }

        // void draw_items(const std::string& name, const std::list<std::string>& items)
        // {
        // }

        void draw_items(const std::string& name, std::vector<std::filesystem::path>& items, const std::vector<dialog_extensions_filter>& filters)
        {
            (void)name;
            (void)items;

            std::string _title = name + " (" + std::to_string(items.size()) + ")" + tag(name + "_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                // static const char* item_names[] = { "Item One", "Item Two", "Item Three", "Item Four", "Item Five", "Item 6", "Item 7" };
                // int selected = 0;

                // float _frame_height_without_padding = ImGui::GetFontSize();
                float _frame_height_without_padding = 30.f;

                if (ImGui::Button("add...")) {
                    std::optional<std::filesystem::path> _picked_file = open_file_dialog(filters);
                    if (_picked_file.has_value()) {
                        std::cout << _picked_file.value() << std::endl;
                        items.push_back(_picked_file.value());
                    }
                }

                float _frame_height = _frame_height_without_padding + 2.f * ImGui::GetStyle().FramePadding.y;
                auto pos = ImGui::GetCursorPos();
                for (int n = 0; n < items.size(); n++) {
                    ImGui::PushID(n);
                    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
                    (ImGui::Selectable(("##" + items[n].generic_string()).c_str(), false, 0, ImVec2(ImGui::GetContentRegionAvail().x, _frame_height_without_padding)));
                    ImGui::SetItemAllowOverlap();
                    if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
                        int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                        if (n_next >= 0 && n_next < items.size()) {
                            std::iter_swap(items.begin() + n, items.begin() + n_next);
                            ImGui::ResetMouseDragDelta();
                        }
                    }
                    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));

                    //
                    //
                    //
                    //
                    static bool vv = false;
                    ImGui::Checkbox("okok", &vv);
                    ImGui::SameLine();
                    ImGui::Text(items[n].generic_string().c_str());
                    //
                    //
                    //
                    //

                    pos.y += _frame_height;
                    ImGui::PopID();
                }
                ImGui::EndTabItem();
            }
        }

    }

    void draw_hotswapper_overlay()
    {
#if BUNGEEGUM_USE_HOTSWAP
        ImGui::SetNextWindowSize({ 300.f, 450.f }, ImGuiCond_Once);
        if (ImGui::Begin("hotswapper##__bungeegum_window_hotswapper_title__", NULL, ImGuiWindowFlags_NoCollapse)) {

            // static const char* item_names[] = { "Item One", "Item Two", "Item Three", "Item Four", "Item Five" };
            // for (int n = 0; n < IM_ARRAYSIZE(item_names); n++) {
            //     const char* item = item_names[n];
            //     static std::string jjj;
            //     ImGui::InputText(item, &jjj);

            //     if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
            //         int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
            //         if (n_next >= 0 && n_next < IM_ARRAYSIZE(item_names)) {
            //             item_names[n] = item_names[n_next];
            //             item_names[n_next] = item;
            //             ImGui::ResetMouseDragDelta();
            //         }
            //     }
            // }

            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar(tag("tab_bar").c_str(), tab_bar_flags)) {
                // draw_items("defines", defines());
                draw_items("include dirs", include_directories(), { { "Library", { "lib", "dll" } } });
                draw_items("source dirs", source_directories(), { { "Library", { "lib", "dll" } } });
                draw_items("source files", force_compiled_source_files(), { { "Library", { "lib", "dll" } } });
                draw_items("libraries", libraries(), { { "Library", { "lib", "dll" } } });
            }
            ImGui::EndTabBar();

            // button ?
            if (ImGui::Button("Trigger hotswap")) {
                // detail::global_manager::backend().reload_manager->force_update();
                throw backtraced_exception(L"LOL");
            }
        }
        ImGui::End();
#endif
    }
}
}
