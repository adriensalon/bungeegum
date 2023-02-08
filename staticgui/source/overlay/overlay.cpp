//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>

#include <staticgui/glue/imguarded.hpp>
#include <staticgui/overlay/overlay.hpp>
#include <staticgui/overlay/widgets/debug.hpp>
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
                overlay_font = io.Fonts->AddFontFromMemoryCompressedTTF(helvetica_compressed_data, helvetica_compressed_size, 13.0f);

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

        void draw_overlay(context_state& context)
        {

            ImGui::PushFont(overlay_font);
            static bool _debug = false;
            static bool _debugcancel = true;
            if (ImGui::GetIO().KeysDown[ImGuiKey_Escape] && _debugcancel) {
                _debug = !_debug;
                _debugcancel = false;
            }
            if (!ImGui::GetIO().KeysDown[ImGuiKey_Escape]) {
                _debugcancel = true;
            }

            // ImGui::ShowDemoWindow();
            if (has_userspace_thrown() || _debug) {
                // set meilleur style

                static bool _setup = false;
                if (!_setup) {
                    ImGui::StyleColorsLight();
                    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
                    ImGuiViewport* viewport = ImGui::GetMainViewport();
                    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
                    ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                    ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags);
                    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                    auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.315f, nullptr, &dockspace_id);
                    auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.315f, nullptr, &dockspace_id);
                    auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.315f, nullptr, &dockspace_id);
                    ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
                    ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
                    ImGui::DockBuilderDockWindow("Profiler", dock_id_down);
                    ImGui::DockBuilderDockWindow("Debug", dockspace_id);
                    ImGui::DockBuilderFinish(dockspace_id);
                    _setup = true;
                }

                draw_debug(context);
                draw_hierarchy(context);
                draw_inspector(context);
                draw_profiler(context);

                ImGui::SetNextWindowFocus();
                ImGui::ShowDemoWindow();
            }
            ImGui::PopFont();
        }
    }
}
}