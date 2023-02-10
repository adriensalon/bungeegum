//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <string>
#include <unordered_map>

#include <imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>

#include <staticgui/overlay/widgets/footer.hpp>

namespace staticgui {
namespace detail {
    namespace overlay {

        static unsigned int vertices_count = 0;
        static unsigned int indices_count = 0;
        static unsigned int commands_count = 0;

        void DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb)
        {
            using namespace ImGui;
            IM_ASSERT(show_mesh || show_aabb);

            // Draw wire-frame version of all triangles
            ImRect clip_rect = draw_cmd->ClipRect;
            ImRect vtxs_rect(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
            ImDrawListFlags backup_flags = out_draw_list->Flags;
            out_draw_list->Flags &= ~ImDrawListFlags_AntiAliasedLines; // Disable AA on triangle outlines is more readable for very large and thin triangles.
            for (unsigned int idx_n = draw_cmd->IdxOffset, idx_end = draw_cmd->IdxOffset + draw_cmd->ElemCount; idx_n < idx_end;) {
                ImDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL; // We don't hold on those pointers past iterations as ->AddPolyline() may invalidate them if out_draw_list==draw_list
                ImDrawVert* vtx_buffer = draw_list->VtxBuffer.Data + draw_cmd->VtxOffset;

                ImVec2 triangle[3];
                for (int n = 0; n < 3; n++, idx_n++)
                    vtxs_rect.Add((triangle[n] = vtx_buffer[idx_buffer ? idx_buffer[idx_n] : idx_n].pos));
                if (show_mesh)
                    out_draw_list->AddPolyline(triangle, 3, IM_COL32(255, 255, 0, 255), ImDrawFlags_Closed, 1.0f); // In yellow: mesh triangles
            }
            // Draw bounding boxes
            if (show_aabb) {
                out_draw_list->AddRect(ImFloor(clip_rect.Min), ImFloor(clip_rect.Max), IM_COL32(255, 0, 255, 255)); // In pink: clipping rectangle submitted to GPU
                out_draw_list->AddRect(ImFloor(vtxs_rect.Min), ImFloor(vtxs_rect.Max), IM_COL32(0, 255, 255, 255)); // In cyan: bounding box of triangles
            }
            out_draw_list->Flags = backup_flags;
        }

        void DebugNodeDrawList(ImGuiWindow* window, ImGuiViewportP* viewport, const ImDrawList* draw_list, const char* label)
        {
            using namespace ImGui;
            ImGuiContext& g = *GImGui;
            ImGuiMetricsConfig* cfg = &g.DebugMetricsConfig;
            int cmd_count = draw_list->CmdBuffer.Size;
            if (cmd_count > 0 && draw_list->CmdBuffer.back().ElemCount == 0 && draw_list->CmdBuffer.back().UserCallback == NULL)
                cmd_count--;
            // bool node_open = TreeNode(draw_list, "%s: '%s' %d vtx, %d indices, %d cmds", label, draw_list->_OwnerName ? draw_list->_OwnerName : "", draw_list->VtxBuffer.Size, draw_list->IdxBuffer.Size, cmd_count);

            // if (draw_list->_OwnerName == "Application") {
            if (draw_list == GetWindowDrawList()) {
                SameLine();
                TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "CURRENTLY APPENDING"); // Can't display stats for active draw list! (we don't have the data double-buffered)
                // if (node_open)
                //     TreePop();
                return;
            }

            ImDrawList* fg_draw_list = viewport ? GetForegroundDrawList(viewport) : NULL; // Render additional visuals into the top-most draw list
            // if (window && IsItemHovered() && fg_draw_list)
            //     fg_draw_list->AddRect(window->Pos, window->Pos + window->Size, IM_COL32(255, 255, 0, 255));
            // if (!node_open)
            //     return;

            if (window && !window->WasActive)
                TextDisabled("Warning: owning Window is inactive. This DrawList is not being rendered!");

            for (const ImDrawCmd* pcmd = draw_list->CmdBuffer.Data; pcmd < draw_list->CmdBuffer.Data + cmd_count; pcmd++) {
                if (pcmd->UserCallback) {
                    BulletText("Callback %p, user_data %p", pcmd->UserCallback, pcmd->UserCallbackData);
                    continue;
                }

                char buf[300];
                ImFormatString(buf, IM_ARRAYSIZE(buf), "DrawCmd:%5d tris, Tex 0x%p, ClipRect (%4.0f,%4.0f)-(%4.0f,%4.0f)",
                    pcmd->ElemCount / 3, (void*)(intptr_t)pcmd->TextureId,
                    pcmd->ClipRect.x, pcmd->ClipRect.y, pcmd->ClipRect.z, pcmd->ClipRect.w);
                // bool pcmd_node_open = TreeNode((void*)(pcmd - draw_list->CmdBuffer.begin()), "%s", buf);
                // if (IsItemHovered() && (cfg->ShowDrawCmdMesh || cfg->ShowDrawCmdBoundingBoxes) && fg_draw_list)
                //     DebugNodeDrawCmdShowMeshAndBoundingBox(fg_draw_list, draw_list, pcmd, cfg->ShowDrawCmdMesh, cfg->ShowDrawCmdBoundingBoxes);
                // if (!pcmd_node_open)
                //     continue;

                // Calculate approximate coverage area (touched pixel count)
                // This will be in pixels squared as long there's no post-scaling happening to the renderer output.
                const ImDrawIdx* idx_buffer = (draw_list->IdxBuffer.Size > 0) ? draw_list->IdxBuffer.Data : NULL;
                const ImDrawVert* vtx_buffer = draw_list->VtxBuffer.Data + pcmd->VtxOffset;
                float total_area = 0.0f;
                for (unsigned int idx_n = pcmd->IdxOffset; idx_n < pcmd->IdxOffset + pcmd->ElemCount;) {
                    ImVec2 triangle[3];
                    for (int n = 0; n < 3; n++, idx_n++)
                        triangle[n] = vtx_buffer[idx_buffer ? idx_buffer[idx_n] : idx_n].pos;
                    total_area += ImTriangleArea(triangle[0], triangle[1], triangle[2]);
                }

                // Display vertex information summary. Hover to get all triangles drawn in wire-frame
                ImFormatString(buf, IM_ARRAYSIZE(buf), "Mesh: ElemCount: %d, VtxOffset: +%d, IdxOffset: +%d, Area: ~%0.f px", pcmd->ElemCount, pcmd->VtxOffset, pcmd->IdxOffset, total_area);
                Selectable(buf);
                // if (IsItemHovered() && fg_draw_list)
                // if (fg_draw_list)
                DebugNodeDrawCmdShowMeshAndBoundingBox(fg_draw_list, draw_list, pcmd, true, false);

                // Display individual triangles/vertices. Hover on to get the corresponding triangle highlighted.
                ImGuiListClipper clipper;
                clipper.Begin(pcmd->ElemCount / 3); // Manually coarse clip our print out of individual vertices to save CPU, only items that may be visible.
                while (clipper.Step())
                    for (int prim = clipper.DisplayStart, idx_i = pcmd->IdxOffset + clipper.DisplayStart * 3; prim < clipper.DisplayEnd; prim++) {
                        char *buf_p = buf, *buf_end = buf + IM_ARRAYSIZE(buf);
                        ImVec2 triangle[3];
                        for (int n = 0; n < 3; n++, idx_i++) {
                            const ImDrawVert& v = vtx_buffer[idx_buffer ? idx_buffer[idx_i] : idx_i];
                            triangle[n] = v.pos;
                            buf_p += ImFormatString(buf_p, buf_end - buf_p, "%s %04d: pos (%8.2f,%8.2f), uv (%.6f,%.6f), col %08X\n",
                                (n == 0) ? "Vert:" : "     ", idx_i, v.pos.x, v.pos.y, v.uv.x, v.uv.y, v.col);
                        }

                        Selectable(buf, false);
                        if (fg_draw_list && IsItemHovered()) {
                            ImDrawListFlags backup_flags = fg_draw_list->Flags;
                            fg_draw_list->Flags &= ~ImDrawListFlags_AntiAliasedLines; // Disable AA on triangle outlines is more readable for very large and thin triangles.
                            fg_draw_list->AddPolyline(triangle, 3, IM_COL32(255, 255, 0, 255), ImDrawFlags_Closed, 1.0f);
                            fg_draw_list->Flags = backup_flags;
                        }
                    }
                // TreePop();
            }
            // TreePop();
            // }
        }

        void ShowMetricsWindow(bool p_open)
        {
            using namespace ImGui;

            ImGuiContext& g = *GImGui;
            ImGuiIO& io = g.IO;
            ImGuiMetricsConfig* cfg = &g.DebugMetricsConfig;

            // if (!Begin("Dear ImGui Metrics/Debugger", p_open) || GetCurrentWindow()->BeginCount > 1) {
            //     End();
            //     return;
            // }

            // Basic info
            // Text("Dear ImGui %s", GetVersion());
            // Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            // Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
            // Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);

            // DrawLists
            int drawlist_count = 0;
            for (int viewport_i = 0; viewport_i < g.Viewports.Size; viewport_i++)
                drawlist_count += g.Viewports[viewport_i]->DrawDataBuilder.GetDrawListCount();
            // Checkbox("Show ImDrawCmd mesh when hovering", &cfg->ShowDrawCmdMesh);
            // Checkbox("Show ImDrawCmd bounding boxes when hovering", &cfg->ShowDrawCmdBoundingBoxes);
            for (int viewport_i = 0; viewport_i < g.Viewports.Size; viewport_i++) {
                ImGuiViewportP* viewport = g.Viewports[viewport_i];
                bool viewport_has_drawlist = false;
                for (int layer_i = 0; layer_i < IM_ARRAYSIZE(viewport->DrawDataBuilder.Layers); layer_i++)
                    for (int draw_list_i = 0; draw_list_i < viewport->DrawDataBuilder.Layers[layer_i].Size; draw_list_i++) {
                        if (std::string(viewport->DrawDataBuilder.Layers[layer_i][draw_list_i]->_OwnerName) == "Application") {
                            ImDrawList* _draw_list = viewport->DrawDataBuilder.Layers[layer_i][draw_list_i];
                            vertices_count = _draw_list->VtxBuffer.Size;
                            indices_count = _draw_list->IdxBuffer.Size;
                            commands_count = _draw_list->CmdBuffer.Size;
                            if (p_open)
                                DebugNodeDrawList(NULL, viewport, _draw_list, "DrawList");
                        }
                    }
            }

            // End();
        }

        void draw_footer(context_state& context)
        {
            static bool _show_metric = true;
            ImGuiViewportP* _viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
            ImGuiWindowFlags _window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
            if (ImGui::BeginViewportSideBar("##MainFooterBar", _viewport, ImGuiDir_Down, ImGui::GetFrameHeight(), _window_flags)) {
                if (ImGui::BeginMenuBar()) {
                    // std::cout << ImGui::GetContentRegionAvail().y << std::endl;
                    std::string _metrics_text(std::to_string(vertices_count) + " vertices, " + std::to_string(indices_count) + " indices (" + std::to_string(commands_count) + " commands)");
                    ImGui::Text(_metrics_text.c_str());
                    ImGui::SameLine();
                    ImGui::Checkbox("Debug", &_show_metric);
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }

            ShowMetricsWindow(_show_metric);
        }
    }
}
}