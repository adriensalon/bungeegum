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
#include <staticgui/overlay/widgets/hierarchy.hpp>
#include <staticgui/overlay/widgets/inspector.hpp>
#include <staticgui/overlay/widgets/profiler.hpp>
#include <staticgui/overlay/widgets/report.hpp>
#include <staticgui/overlay/widgets/wireframe.hpp>
#include <staticgui/state/errors.hpp>

#include "embedded/fa4.cpp"
#include "embedded/fa4.hpp"
#include "embedded/helvetica.cpp"

namespace staticgui {
namespace detail {
    namespace overlay {

        static ImFont* overlay_font = nullptr;
        static ImFont* icons_font = nullptr;
        static bool show_hierarchy = false;
        static bool show_inspector = false;
        static bool show_profiler = false;
        static bool show_wireframe = false;

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

        void draw_dockspace(const std::function<void(ImDrawList*)>& draw_commands)
        {
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y - ImGui::GetFrameHeight() });
            ImGui::SetNextWindowViewport(viewport->ID);
            {
                glue::style_guard _sg0(ImGuiStyleVar_WindowRounding, 0.f);
                glue::style_guard _sg1(ImGuiStyleVar_WindowBorderSize, 0.f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                    window_flags |= ImGuiWindowFlags_NoBackground;
                glue::style_guard _sg2(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
                ImGui::Begin("Viewport", nullptr, window_flags);
            }
            draw_commands(ImGui::GetWindowDrawList());
            if (show_wireframe)
                draw_wireframe();
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            static auto first_time = true;
            if (first_time) {
                first_time = false;
                ImGui::DockBuilderSetNodeSize(dockspace_id, { viewport->Size.x, viewport->Size.y - ImGui::GetFrameHeight() });
                ImGui::DockBuilderFinish(dockspace_id);
            }
            ImGui::End();
        }

        void draw_footer()
        {
            ImGuiViewportP* _viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
            ImGuiWindowFlags _window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
            if (ImGui::BeginViewportSideBar("##MainFooterBar", _viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), _window_flags)) {
                if (ImGui::BeginMenuBar()) {
                    if (ImGui::Button("hierarchy##__staticgui_footer_hierarchy_button__")) {
                        show_hierarchy = !show_hierarchy;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("inspector##__staticgui_footer_inspector_button__")) {
                        show_inspector = !show_inspector;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("profiler##__staticgui_footer_profiler_button__")) {
                        show_profiler = !show_profiler;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("wireframe##__staticgui_footer_wireframe_button__")) {
                        show_wireframe = !show_wireframe;
                    }
                    ImGui::SameLine();
                    std::string _metrics_text(std::to_string(shared_data::vertices_count) + " vertices, " + std::to_string(shared_data::indices_count) + " indices (" + std::to_string(shared_data::commands_count) + " commands)");
                    ImGui::Text(_metrics_text.c_str());
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }
        }

        void draw_overlay(context_state& context, const std::function<void(ImDrawList*)>& draw_commands)
        {
            ImGui::PushFont(overlay_font);

            draw_dockspace(draw_commands);
            draw_footer();
            if (show_hierarchy)
                draw_hierarchy(context);
            if (show_inspector)
                draw_inspector(context);
            if (show_profiler)
                draw_profiler(context);

            ImGui::ShowDemoWindow();

            ImGui::PopFont();
        }
    }
}
}