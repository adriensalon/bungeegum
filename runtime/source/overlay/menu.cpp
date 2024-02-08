#include <bungeegum/config/feature.hpp>
#if BUNGEEGUM_USE_OVERLAY

#include <imgui.h>
#include <imgui_internal.h>

#include <functional>

#include <bungeegum/glue/imutil.hpp>

#include "compressed/font_awesome.hpp"
#include "compressed/inter_extrabold.cpp"
#include "compressed/inter_regular.cpp"

namespace bungeegum {
namespace detail {
	
    extern void draw_build_overlay();
    extern void draw_hierarchy_overlay();
    extern void draw_hotswap_overlay();
    extern void draw_output_overlay();
    extern void draw_profiling_overlay();

    namespace {

        ImFont* regular_font = nullptr;
        ImFont* extrabold_font = nullptr;
        ImFont* icons_font = nullptr;

        bool show_build_overlay = false;
        bool show_hierarchy_overlay = false;
        bool show_hotswap_overlay = false;
        bool show_output_overlay = false;
        bool show_profiling_overlay = false;

        unsigned int vertices_count = 0;
        unsigned int indices_count = 0;
        unsigned int commands_count = 0;

        float footer_height = 28.f;

		void with_theme(const std::function<void()>& callback)
		{
			font_guard _fguard00(regular_font);

			color_guard _cguard00(ImGuiCol_WindowBg, { 0.878f, 0.878f, 0.878f, 1.f });
            color_guard _cguard01(ImGuiCol_ScrollbarBg, { 0.878f, 0.878f, 0.878f, 1.f });
            color_guard _cguard02(ImGuiCol_TitleBg, { 0.627f, 0.627f, 0.627f, 1.f });
            color_guard _cguard03(ImGuiCol_TitleBgActive, { 0.627f, 0.627f, 0.627f, 1.f });
            color_guard _cguard04(ImGuiCol_Text, { 0.023f, 0.023f, 0.023f, 1.f });
            color_guard _cguard05(ImGuiCol_FrameBg, { 0.980f, 0.980f, 0.980f, 1.f });
            color_guard _cguard06(ImGuiCol_PopupBg, { 0.980f, 0.980f, 0.980f, 1.f });
            color_guard _cguard07(ImGuiCol_TableHeaderBg, { 0.f, 0.f, 0.f, 0.f });
            color_guard _cguard08(ImGuiCol_TableBorderStrong, { 0.f, 0.f, 0.f, 0.f });
            color_guard _cguard09(ImGuiCol_Border, { 0.627f, 0.627f, 0.627f, 1.f });
            color_guard _cguard10(ImGuiCol_Tab, { 0.58f, 0.58f, 0.58f, 0.40f });
            color_guard _cguard11(ImGuiCol_TabActive, { 0.627f, 0.627f, 0.627f, 1.f });
            color_guard _cguard12(ImGuiCol_TabUnfocusedActive, { 0.627f, 0.627f, 0.627f, 1.f });
            color_guard _cguard13(ImGuiCol_Button, { 0.878f, 0.878f, 0.878f, 1.f });
            color_guard _cguard14(ImGuiCol_HeaderHovered, { 0.f, 0.f, 0.f, 0.f });
            color_guard _cguard15(ImGuiCol_HeaderActive, { 0.f, 0.f, 0.f, 0.f });
			
			style_guard _sguard00(ImGuiStyleVar_WindowBorderSize, 0.f);
            style_guard _sguard01(ImGuiStyleVar_ChildBorderSize, 0.f);
            style_guard _sguard02(ImGuiStyleVar_PopupBorderSize, 0.f);
            style_guard _sguard03(ImGuiStyleVar_FrameBorderSize, 0.f);
            style_guard _sguard04(ImGuiStyleVar_WindowRounding, 3.f);
            style_guard _sguard05(ImGuiStyleVar_ChildRounding, 3.f);
            style_guard _sguard06(ImGuiStyleVar_FrameRounding, 3.f);
            style_guard _sguard07(ImGuiStyleVar_PopupRounding, 3.f);
            style_guard _sguard08(ImGuiStyleVar_ScrollbarRounding, 3.f);
            style_guard _sguard09(ImGuiStyleVar_GrabRounding, 4.f);
            style_guard _sguard10(ImGuiStyleVar_TabRounding, 4.f);
            style_guard _sguard11(ImGuiStyleVar_ItemSpacing, { 5.f, 5.f });
            style_guard _sguard12(ImGuiStyleVar_WindowTitleAlign, { 0.5f, 0.5f });
			
			callback();
		}
		
		void with_additional_theme(const std::function<void()>& callback)
		{
			style_guard _sguard00(ImGuiStyleVar_FramePadding, { 4.f, 3.3f });
            style_guard _sguard01(ImGuiStyleVar_WindowBorderSize, 1.5f);			
			callback();
		}

        void update_stats()
        {
            using namespace ImGui;
            ImGuiContext& _g = *GImGui;
            for (int viewport_i = 0; viewport_i < _g.Viewports.Size; viewport_i++) {
                ImGuiViewportP* viewport = _g.Viewports[viewport_i];
                for (int layer_i = 0; layer_i < IM_ARRAYSIZE(viewport->DrawDataBuilder.Layers); layer_i++) {
                    for (int draw_list_i = 0; draw_list_i < viewport->DrawDataBuilder.Layers[layer_i].Size; draw_list_i++) {
                        if (std::string(viewport->DrawDataBuilder.Layers[layer_i][draw_list_i]->_OwnerName) == "Viewport") {
                            ImDrawList* _draw_list = viewport->DrawDataBuilder.Layers[layer_i][draw_list_i];
                            vertices_count = _draw_list->VtxBuffer.Size;
                            indices_count = _draw_list->IdxBuffer.Size;
                            commands_count = _draw_list->CmdBuffer.Size;
                        }
                    }
                }
            }
        }

        void draw_footer_button(bool& state, const std::string_view& name)
        {
            font_guard _fg_selected;
            if (state)
                _fg_selected.set(extrabold_font);
            std::string _name_str = name.data() + std::string("##__bungeegum_footer_button_") + name.data();
            if (ImGui::Button(_name_str.c_str())) {
                state = !state;
            }
            _fg_selected.release();
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

                    draw_footer_button(show_build_overlay, "build");
                    ImGui::SameLine();
                    draw_footer_button(show_hierarchy_overlay, "hierarchy");
                    ImGui::SameLine();
                    draw_footer_button(show_hotswap_overlay, "hotswap");
                    ImGui::SameLine();
                    draw_footer_button(show_output_overlay, "output");
                    ImGui::SameLine();
                    draw_footer_button(show_profiling_overlay, "profiling");
                    ImGui::SameLine();

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (footer_height - ImGui::GetFrameHeight()) / 2.f);
                    std::string _metrics_text_1(std::to_string(vertices_count) + " vertices, " + std::to_string(indices_count) + " indices,");
                    std::string _metrics_text_2(std::to_string(commands_count) + " draw calls");
                    float _metrics_size_1 = ImGui::CalcTextSize(_metrics_text_1.c_str()).x + 5.f;
                    float _metrics_size_2 = ImGui::CalcTextSize(_metrics_text_2.c_str()).x;
                    ImVec2 _cursor_pos_mem = ImGui::GetCursorPos();
                    ImGui::SetCursorPosX(_cursor_pos_mem.x + ImGui::GetContentRegionAvail().x - _metrics_size_1 - _metrics_size_2 - ImGui::GetStyle().ItemSpacing.x);
                    ImGui::Text(_metrics_text_1);
                    ImGui::SetCursorPosY(_cursor_pos_mem.y);
                    {
                        font_guard _fglast(extrabold_font);
                        ImGui::Text(_metrics_text_2);
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }
        }
    }

    void setup_overlay()
    {
        // setup_profiler_overlay();

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

    void draw_overlay()
    {
		with_theme([] () {
			update_stats();
			draw_footer();
			with_additional_theme([] () {
        		if (show_build_overlay) {
					draw_build_overlay();
				}
				if (show_hierarchy_overlay) {
					draw_hierarchy_overlay();
				}
				if (show_hotswap_overlay) {
					draw_hotswap_overlay();
				}
				if (show_output_overlay) {
					draw_output_overlay();
				}
				if (show_profiling_overlay) {
					draw_profiling_overlay();
				}
			});
		});
    }
}
}

#endif