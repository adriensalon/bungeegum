#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/glue/imguarded.fwd>

#include "compressed/fa4.cpp"
#include "compressed/fa4.hpp"
#include "compressed/inter_extrabold.cpp"
#include "compressed/inter_regular.cpp"

namespace bungeegum {
namespace detail {

    namespace {
        static unsigned int vertices_count = 0;
        static unsigned int indices_count = 0;
        static unsigned int commands_count = 0;

        static bool show_hierarchy = false;
        static bool show_inspector = false;
        static bool show_profiler = false;
        static bool show_wireframe = false;
        static bool show_hotswapper = false;
        static bool show_logger = false;
    }

    void setup_overlay()
    {
        setup_profiler_overlay();

        static bool _installed = false;
        if (!_installed) {

            // config.
            ImGuiIO& io = ImGui::GetIO();
            // io.Fonts->AddFontFromMemoryCompressedTTF(helvetica_compressed_data, helvetica_compressed_size, 13.0f);
            regular_font = io.Fonts->AddFontFromMemoryCompressedTTF(inter_regular_compressed_data, inter_regular_compressed_size, 13.0f);
            extrabold_font = io.Fonts->AddFontFromMemoryCompressedTTF(inter_extrabold_compressed_data, inter_extrabold_compressed_size, 13.0f);
            // shared_data::regular_font = io.Fonts->AddFontFromMemoryCompressedTTF(inter_extrabold_compressed_data, inter_extrabold_compressed_size, 13.0f);

            // font awesome for the glyphs
            // ImFontConfig config;
            // config.MergeMode = true;
            // config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
            // static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            // icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(fa4_compressed_data, fa4_compressed_size, 13.0f, &config, icon_ranges);

            // build
            io.Fonts->Build();
            io.FontDefault = regular_font;
            _installed = true;
        }
    }

    void draw_dockspace(const std::function<void(ImDrawList*)>& draw_commands)
    {
        // static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        // ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        // ImGuiViewport* viewport = ImGui::GetMainViewport();
        // ImGui::SetNextWindowPos(viewport->Pos);
        // ImGui::SetNextWindowSize({ viewport->Size.x, viewport->Size.y - footer_height });
        // ImGui::SetNextWindowViewport(viewport->ID);
        // {
        //     style_guard _sg0(ImGuiStyleVar_WindowRounding, 0.f);
        //     style_guard _sg1(ImGuiStyleVar_WindowBorderSize, 0.f);
        //     window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        //     window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        //     if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        //         window_flags |= ImGuiWindowFlags_NoBackground;
        //     style_guard _sg2(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        //     ImGui::Begin("Viewport", nullptr, window_flags);
        // }
        // draw_commands(ImGui::GetWindowDrawList());
        draw_commands(ImGui::GetBackgroundDrawList());
        if (show_wireframe)
            draw_wireframe_overlay();
        // ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        // ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        // static auto first_time = true;
        // if (first_time) {
        //     first_time = false;
        //     ImGui::DockBuilderSetNodeSize(dockspace_id, { viewport->Size.x, viewport->Size.y - footer_height });
        //     ImGui::DockBuilderFinish(dockspace_id);
        // }
        // ImGui::End();
    }

    void draw_footer()
    {
        ImGuiViewportP* _viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
        ImGuiWindowFlags _window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        style_guard _sg0(ImGuiStyleVar_FramePadding, { ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y });
        color_guard _cg1(ImGuiCol_MenuBarBg, { 0.878f, 0.878f, 0.878f, 1.f });
        if (ImGui::BeginViewportSideBar("##MainFooterBar", _viewport, ImGuiDir_Down, footer_height, _window_flags)) {

            if (ImGui::BeginMenuBar()) {
                _sg0.release();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (footer_height - ImGui::GetFrameHeight()) / 2.f);

                font_guard _fg0;
                if (show_hierarchy)
                    _fg0.set(extrabold_font);
                if (ImGui::Button("hierarchy##__bungeegum_footer_hierarchy_button__")) {
                    show_hierarchy = !show_hierarchy;
                }
                _fg0.release();
                ImGui::SameLine();

                font_guard _fg1;
                if (show_inspector)
                    _fg1.set(extrabold_font);
                if (ImGui::Button("inspector##__bungeegum_footer_inspector_button__")) {
                    show_inspector = !show_inspector;
                }
                _fg1.release();
                ImGui::SameLine();

                font_guard _fg2;
                if (show_profiler)
                    _fg2.set(extrabold_font);
                if (ImGui::Button("profiler##__bungeegum_footer_profiler_button__")) {
                    show_profiler = !show_profiler;
                }
                _fg2.release();
                ImGui::SameLine();

                font_guard _fg_hotswapper;
                if (show_hotswapper)
                    _fg_hotswapper.set(extrabold_font);
                if (ImGui::Button("hotswapper##__bungeegum_footer_hotswapper_button__")) {
                    show_hotswapper = !show_hotswapper;
                }
                _fg_hotswapper.release();
                ImGui::SameLine();

                font_guard _fg_logger;
                if (show_logger)
                    _fg_logger.set(extrabold_font);
                if (ImGui::Button("logger##__bungeegum_footer_logger_button__")) {
                    show_logger = !show_logger;
                }
                _fg_logger.release();
                ImGui::SameLine();

                font_guard _fg3;
                if (show_wireframe)
                    _fg3.set(extrabold_font);
                if (ImGui::Button("wireframe##__bungeegum_footer_wireframe_button__")) {
                    show_wireframe = !show_wireframe;
                }
                _fg3.release();
                ImGui::SameLine();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (footer_height - ImGui::GetFrameHeight()) / 2.f);
                std::string _metrics_text_1(std::to_string(vertices_count) + " vertices, " + std::to_string(indices_count) + " indices,");
                std::string _metrics_text_2(std::to_string(commands_count) + " draw calls");
                float _metrics_size_1 = ImGui::CalcTextSize(_metrics_text_1.c_str()).x + 5.f;
                float _metrics_size_2 = ImGui::CalcTextSize(_metrics_text_2.c_str()).x;
                ImVec2 _cursor_pos_mem = ImGui::GetCursorPos();
                ImGui::SetCursorPosX(_cursor_pos_mem.x + ImGui::GetContentRegionAvail().x - _metrics_size_1 - _metrics_size_2 - ImGui::GetStyle().ItemSpacing.x);
                ImGui::Text(_metrics_text_1.c_str());
                ImGui::SetCursorPosY(_cursor_pos_mem.y);
                {
                    font_guard _fglast(extrabold_font);
                    ImGui::Text(_metrics_text_2.c_str());
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    }

    void draw_overlay(const std::function<void(ImDrawList*)>& draw_commands)
    {
#if !defined(BUNGEEGUM_ENABLE_OVERLAY)
        draw_commands(ImGui::GetForegroundDrawList());
#else
        // ImGui::ShowDemoWindow();
        font_guard _fg0(regular_font);

        color_guard _cg0(ImGuiCol_WindowBg, { 0.878f, 0.878f, 0.878f, 1.f });
        color_guard _cg_scrollbar_bg(ImGuiCol_ScrollbarBg, { 0.878f, 0.878f, 0.878f, 1.f });
        color_guard _cg1(ImGuiCol_TitleBg, { 0.627f, 0.627f, 0.627f, 1.f });
        color_guard _cg2(ImGuiCol_TitleBgActive, { 0.627f, 0.627f, 0.627f, 1.f });
        color_guard _cg3(ImGuiCol_Text, { 0.023f, 0.023f, 0.023f, 1.f });
        color_guard _cg6(ImGuiCol_FrameBg, { 0.980f, 0.980f, 0.980f, 1.f });
        color_guard _cg7(ImGuiCol_PopupBg, { 0.980f, 0.980f, 0.980f, 1.f });
        color_guard _cg8(ImGuiCol_TableHeaderBg, { 0.f, 0.f, 0.f, 0.f });
        color_guard _cg9(ImGuiCol_TableBorderStrong, { 0.f, 0.f, 0.f, 0.f });
        color_guard _cg_border(ImGuiCol_Border, { 0.627f, 0.627f, 0.627f, 1.f });
        color_guard _cg_tab(ImGuiCol_Tab, { 0.58f, 0.58f, 0.58f, 0.40f });
        color_guard _cg_tab_active(ImGuiCol_TabActive, { 0.627f, 0.627f, 0.627f, 1.f });
        color_guard _cg_tab_unfocused_active(ImGuiCol_TabUnfocusedActive, { 0.627f, 0.627f, 0.627f, 1.f });
        color_guard _cg_button(ImGuiCol_Button, { 0.878f, 0.878f, 0.878f, 1.f });
        color_guard _invisible_header_hovered(ImGuiCol_HeaderHovered, { 0.f, 0.f, 0.f, 0.f });
        color_guard _invisible_header_active(ImGuiCol_HeaderActive, { 0.f, 0.f, 0.f, 0.f });

        // border
        style_guard _sg0(ImGuiStyleVar_WindowBorderSize, 0.f);
        style_guard _sg1(ImGuiStyleVar_ChildBorderSize, 0.f);
        style_guard _sg2(ImGuiStyleVar_PopupBorderSize, 0.f);
        style_guard _sg3(ImGuiStyleVar_FrameBorderSize, 0.f);

        // rounding
        style_guard _sg4(ImGuiStyleVar_WindowRounding, 3.f);
        style_guard _sg5(ImGuiStyleVar_ChildRounding, 3.f);
        style_guard _sg6(ImGuiStyleVar_FrameRounding, 3.f);
        style_guard _sg7(ImGuiStyleVar_PopupRounding, 3.f);
        style_guard _sg8(ImGuiStyleVar_ScrollbarRounding, 3.f);
        style_guard _sg9(ImGuiStyleVar_GrabRounding, 4.f);
        style_guard _sg10(ImGuiStyleVar_TabRounding, 4.f);

        style_guard _sg12(ImGuiStyleVar_ItemSpacing, { 5.f, 5.f });
        style_guard _sg13(ImGuiStyleVar_WindowTitleAlign, { 0.5f, 0.5f });

        draw_dockspace(draw_commands);

        using namespace ImGui;
        ImGuiContext& g = *GImGui;
        int drawlist_count = 0;
        for (int viewport_i = 0; viewport_i < g.Viewports.Size; viewport_i++)
            drawlist_count += g.Viewports[viewport_i]->DrawDataBuilder.GetDrawListCount();
        for (int viewport_i = 0; viewport_i < g.Viewports.Size; viewport_i++) {
            ImGuiViewportP* viewport = g.Viewports[viewport_i];
            for (int layer_i = 0; layer_i < IM_ARRAYSIZE(viewport->DrawDataBuilder.Layers); layer_i++)
                for (int draw_list_i = 0; draw_list_i < viewport->DrawDataBuilder.Layers[layer_i].Size; draw_list_i++) {
                    if (std::string(viewport->DrawDataBuilder.Layers[layer_i][draw_list_i]->_OwnerName) == "Viewport") {
                        ImDrawList* _draw_list = viewport->DrawDataBuilder.Layers[layer_i][draw_list_i];
                        vertices_count = _draw_list->VtxBuffer.Size;
                        indices_count = _draw_list->IdxBuffer.Size;
                        commands_count = _draw_list->CmdBuffer.Size;
                    }
                }
        }

        draw_footer();

        style_guard _sg11(ImGuiStyleVar_FramePadding, { 4.f, 3.3f });
        style_guard _sg_window_border_size(ImGuiStyleVar_WindowBorderSize, 1.5f);

        if (show_hierarchy) {
            draw_hierarchy_overlay();
        }
        if (show_inspector) {
            draw_inspector_overlay();
        }
        if (show_profiler) {
            draw_profiler_overlay();
        }
        if (show_hotswapper) {
            draw_hotswapper_overlay();
        }
        if (show_logger) {
            draw_logger_overlay();
        }

        // ImGui::ShowDemoWindow();
#endif
    }
}

}