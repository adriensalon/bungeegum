//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>

#include <staticgui/glue/imguarded.hpp>
#include <staticgui/overlay/overlay.hpp>
#include <staticgui/overlay/widgets/debug.hpp>
#include <staticgui/overlay/widgets/footer.hpp>
#include <staticgui/overlay/widgets/hierarchy.hpp>
#include <staticgui/overlay/widgets/inspector.hpp>
#include <staticgui/overlay/widgets/profiler.hpp>
#include <staticgui/state/errors.hpp>

#include "embedded/fa4.cpp"
#include "embedded/fa4.hpp"
#include "embedded/helvetica.cpp"

namespace staticgui {
namespace detail {
    namespace overlay {

        static ImFont* overlay_font = nullptr;
        static ImFont* icons_font = nullptr;

        void setup_overlay(context_state& context)
        {
            setup_profiler(context);

            static bool _installed = false;
            if (!_installed) {

                // config.
                ImGuiIO& io = ImGui::GetIO();
                // io.Fonts->AddFontFromMemoryCompressedTTF(helvetica_compressed_data, helvetica_compressed_size, 13.0f);
                overlay_font = io.Fonts->AddFontFromMemoryCompressedTTF(helvetica_compressed_data, helvetica_compressed_size, 11.0f);

                // font awesome for the glyphs
                ImFontConfig config;
                config.MergeMode = true;
                config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
                static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
                icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(fa4_compressed_data, fa4_compressed_size, 13.0f, &config, icon_ranges);

                // build
                io.Fonts->Build();
                io.FontDefault = overlay_font;
                _installed = true;
            }
        }

        void draw_overlay(context_state& context, bool& show, const std::function<void(ImDrawList*)>& draw_commands)
        {
            ImGui::PushFont(overlay_font);

            static bool _show_mem = true;
            if (ImGui::GetIO().KeysDown[ImGuiKey_Escape] && _show_mem) {
                show = !show;
                _show_mem = false;
            }
            if (!ImGui::GetIO().KeysDown[ImGuiKey_Escape]) {
                _show_mem = true;
            }

            if (has_userspace_thrown())
                show = true;

            if (show) {
                static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
                // ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                // ImGui::SetNextWindowPos({ 0.f, 0.f });
                // ImGui::SetNextWindowSize(viewport->Size);
                // ImGui::SetNextWindowBgAlpha(0.f);
                ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y - 17 });
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
                    // window_flags |= ImGuiWindowFlags_NoBackground;
                }

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("DockSpace", nullptr, window_flags);
                ImGui::PopStyleVar();
                ImGui::PopStyleVar(2);

                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
                static auto first_time = true;
                if (first_time) {
                    first_time = false;
                    ImGui::DockBuilderRemoveNode(dockspace_id);
                    ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                    ImGui::DockBuilderSetNodeSize(dockspace_id, { viewport->Size.x, viewport->Size.y - 17 });

                    auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
                    auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
                    auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.3f, nullptr, &dockspace_id);
                    ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
                    ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
                    ImGui::DockBuilderDockWindow("Profiler", dock_id_down);
                    ImGui::DockBuilderDockWindow("Viewport", dockspace_id);

                    ImGui::DockBuilderFinish(dockspace_id);
                }
                ImGui::End();

                ImGui::ShowDemoWindow();

                draw_footer(context);
                draw_hierarchy(context);
                draw_inspector(context);
                draw_profiler(context);
                // draw_debug(context);

                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4 { 1.f, 1.f, 1.f, 1.f });
                if (ImGui::Begin("Viewport")) {
                    draw_commands(ImGui::GetWindowDrawList());
                }
                ImGui::End();
                ImGui::PopStyleColor();
            } else {
                draw_commands(ImGui::GetBackgroundDrawList());
            }
            ImGui::PopFont();
        }
    }
}
}