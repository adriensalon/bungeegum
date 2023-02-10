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
#include "embedded/inter_extrabold.cpp"
#include "embedded/inter_regular.cpp"
#include "embedded/inter_thin.cpp"

namespace staticgui {
namespace detail {
    namespace overlay {

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
                shared_data::regular_font = io.Fonts->AddFontFromMemoryCompressedTTF(inter_regular_compressed_data, inter_regular_compressed_size, 13.0f);
                shared_data::extrabold_font = io.Fonts->AddFontFromMemoryCompressedTTF(inter_extrabold_compressed_data, inter_extrabold_compressed_size, 13.0f);
                // shared_data::regular_font = io.Fonts->AddFontFromMemoryCompressedTTF(inter_extrabold_compressed_data, inter_extrabold_compressed_size, 13.0f);

                // font awesome for the glyphs
                // ImFontConfig config;
                // config.MergeMode = true;
                // config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
                // static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
                // icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(fa4_compressed_data, fa4_compressed_size, 13.0f, &config, icon_ranges);

                // build
                io.Fonts->Build();
                io.FontDefault = shared_data::regular_font;
                _installed = true;
            }
        }

        void draw_dockspace(const std::function<void(ImDrawList*)>& draw_commands)
        {
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y - footer_height });
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
                ImGui::DockBuilderSetNodeSize(dockspace_id, { viewport->Size.x, viewport->Size.y - footer_height });
                ImGui::DockBuilderFinish(dockspace_id);
            }
            ImGui::End();
        }

        void draw_footer()
        {
            ImGuiViewportP* _viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
            ImGuiWindowFlags _window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
            glue::style_guard _sg0(ImGuiStyleVar_FramePadding, { ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y });
            // glue::color_guard _cg0(ImGuiCol_Button, { 0.627f, 0.627f, 0.627f, 1.f });
            glue::color_guard _cg1(ImGuiCol_MenuBarBg, { 0.878f, 0.878f, 0.878f, 1.f });
            glue::color_guard _cg2(ImGuiCol_Button, { 0.878f, 0.878f, 0.878f, 1.f });
            if (ImGui::BeginViewportSideBar("##MainFooterBar", _viewport, ImGuiDir_Down, footer_height, _window_flags)) {

                if (ImGui::BeginMenuBar()) {
                    _sg0.release();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (footer_height - ImGui::GetFrameHeight()) / 2.f);

                    glue::font_guard _fg0;
                    if (show_hierarchy)
                        _fg0.set(shared_data::extrabold_font);
                    if (ImGui::Button("hierarchy##__staticgui_footer_hierarchy_button__")) {
                        show_hierarchy = !show_hierarchy;
                    }
                    _fg0.release();
                    ImGui::SameLine();

                    glue::font_guard _fg1;
                    if (show_inspector)
                        _fg1.set(shared_data::extrabold_font);
                    if (ImGui::Button("inspector##__staticgui_footer_inspector_button__")) {
                        show_inspector = !show_inspector;
                    }
                    _fg1.release();
                    ImGui::SameLine();

                    glue::font_guard _fg2;
                    if (show_profiler)
                        _fg2.set(shared_data::extrabold_font);
                    if (ImGui::Button("profiler##__staticgui_footer_profiler_button__")) {
                        show_profiler = !show_profiler;
                    }
                    _fg2.release();
                    ImGui::SameLine();

                    glue::font_guard _fg3;
                    if (show_wireframe)
                        _fg3.set(shared_data::extrabold_font);
                    if (ImGui::Button("wireframe##__staticgui_footer_wireframe_button__")) {
                        show_wireframe = !show_wireframe;
                    }
                    _fg3.release();
                    ImGui::SameLine();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (footer_height - ImGui::GetFrameHeight()) / 2.f);
                    std::string _metrics_text(std::to_string(shared_data::vertices_count) + " vertices, " + std::to_string(shared_data::indices_count) + " indices (" + std::to_string(shared_data::commands_count) + " commands)");
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(_metrics_text.c_str()).x);
                    ImGui::Text(_metrics_text.c_str());
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }
        }

        void draw_overlay(context_state& context, const std::function<void(ImDrawList*)>& draw_commands)
        {
            ImGui::PushFont(shared_data::regular_font);

            glue::color_guard _cg0(ImGuiCol_WindowBg, { 0.878f, 0.878f, 0.878f, 1.f });
            glue::color_guard _cg1(ImGuiCol_TitleBg, { 0.627f, 0.627f, 0.627f, 1.f });
            glue::color_guard _cg2(ImGuiCol_TitleBgActive, { 0.627f, 0.627f, 0.627f, 1.f });
            glue::color_guard _cg3(ImGuiCol_Text, { 0.023f, 0.023f, 0.023f, 1.f });
            glue::color_guard _cg4(ImGuiCol_Tab, { 0.878f, 0.878f, 0.878f, 1.f });
            glue::color_guard _cg5(ImGuiCol_TabUnfocusedActive, { 0.878f, 0.878f, 0.878f, 1.f });
            glue::color_guard _cg6(ImGuiCol_FrameBg, { 0.980f, 0.980f, 0.980f, 1.f });
            glue::color_guard _cg7(ImGuiCol_PopupBg, { 0.980f, 0.980f, 0.980f, 1.f });

            // border
            glue::style_guard _sg0(ImGuiStyleVar_WindowBorderSize, 0.f);
            glue::style_guard _sg1(ImGuiStyleVar_ChildBorderSize, 0.f);
            glue::style_guard _sg2(ImGuiStyleVar_PopupBorderSize, 0.f);
            glue::style_guard _sg3(ImGuiStyleVar_FrameBorderSize, 0.f);

            // rounding
            glue::style_guard _sg4(ImGuiStyleVar_WindowRounding, 3.f);
            glue::style_guard _sg5(ImGuiStyleVar_ChildRounding, 3.f);
            glue::style_guard _sg6(ImGuiStyleVar_FrameRounding, 3.f);
            glue::style_guard _sg7(ImGuiStyleVar_PopupRounding, 3.f);
            glue::style_guard _sg8(ImGuiStyleVar_ScrollbarRounding, 3.f);
            glue::style_guard _sg9(ImGuiStyleVar_GrabRounding, 4.f);
            glue::style_guard _sg10(ImGuiStyleVar_TabRounding, 4.f);

            glue::style_guard _sg12(ImGuiStyleVar_ItemSpacing, { 5.f, 5.f });
            glue::style_guard _sg13(ImGuiStyleVar_WindowTitleAlign, { 0.5f, 0.5f });

            draw_dockspace(draw_commands);
            draw_footer();

            glue::style_guard _sg11(ImGuiStyleVar_FramePadding, { 4.f, 4.f });

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