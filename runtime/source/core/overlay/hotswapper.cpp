#include <algorithm>
#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/backend/backend.hpp>
#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/backtrace.hpp>
#include <bungeegum/glue/imguarded.fwd>

namespace bungeegum {
namespace detail {

    namespace {

        std::string tag(const std::string& name)
        {
            return "##__bungeegum_overlay_hotswapper_" + name + "__";
        }

        // void draw_items(const std::string& name, const std::list<std::string>& items)
        // {
        // }

        void draw_items(const std::string& name, std::vector<std::filesystem::path>& items)
        {
            (void)name;
            (void)items;

            std::string _title = name + " (" + std::to_string(items.size()) + ")" + tag(name + "_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {
                // static const char* item_names[] = { "Item One", "Item Two", "Item Three", "Item Four", "Item Five", "Item 6", "Item 7" };
                int selected = 0;
                auto pos = ImGui::GetCursorPos();

                // selectable list
                for (int n = 0; n < items.size(); n++) {
                    // const char* item = items[n].generic_string().c_str();
                    ImGui::PushID(n);

                    char buf[32];
                    sprintf(buf, "##Object %d", n);

                    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
                    if (ImGui::Selectable(("##" + items[n].generic_string()).c_str(), n == selected, 0, ImVec2(ImGui::GetContentRegionAvail().x, 50))) {
                        selected = n;
                    }
                    ImGui::SetItemAllowOverlap();
                    if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
                        int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                        if (n_next >= 0 && n_next < items.size()) {
                            // item_names[n] = item_names[n_next];
                            // item_names[n_next] = item;

                            std::iter_swap(items.begin() + n, items.begin() + n_next);
                            ImGui::ResetMouseDragDelta();
                        }
                    }

                    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
                    ImGui::Text(items[n].generic_string().c_str());

                    ImGui::SetCursorPos(ImVec2(pos.x + 30, pos.y + 5));

                    ImGui::SetCursorPos(ImVec2(pos.x, pos.y + 20));
                    // ImGui::Text("bar");

                    pos.y += 55;

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
                draw_items("include dirs", include_directories());
                draw_items("source dirs", source_directories());
                draw_items("source files", force_compiled_source_files());
                draw_items("libraries", libraries());
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
