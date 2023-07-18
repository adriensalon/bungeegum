#include <string>
#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>

#include <bungeegum/core/overlay.fwd>

namespace bungeegum {
namespace detail {

    void DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd)
    {
        using namespace ImGui;
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
            out_draw_list->AddPolyline(triangle, 3, IM_COL32(255, 255, 0, 255), ImDrawFlags_Closed, 1.0f); // In yellow: mesh triangles
        }
        out_draw_list->Flags = backup_flags;
    }

    void DebugNodeDrawList(ImGuiViewportP* viewport, const ImDrawList* draw_list)
    {
        using namespace ImGui;
        ImDrawList* fg_draw_list = GetForegroundDrawList(); // viewport ? GetForegroundDrawList(viewport) : NULL; // Render additional visuals into the top-most draw list
        for (const ImDrawCmd* pcmd = draw_list->CmdBuffer.Data; pcmd < draw_list->CmdBuffer.Data + draw_list->CmdBuffer.Size; pcmd++) {
            DebugNodeDrawCmdShowMeshAndBoundingBox(fg_draw_list, draw_list, pcmd);
        }
        (void)viewport;
    }

    void draw_wireframe_overlay()
    {
        using namespace ImGui;
        ImGuiContext& g = *GImGui;
        int drawlist_count = 0;
        for (int viewport_i = 0; viewport_i < g.Viewports.Size; viewport_i++)
            drawlist_count += g.Viewports[viewport_i]->DrawDataBuilder.GetDrawListCount();
        for (int viewport_i = 0; viewport_i < g.Viewports.Size; viewport_i++) {
            ImGuiViewportP* viewport = g.Viewports[viewport_i];
            DebugNodeDrawList(viewport, ImGui::GetBackgroundDrawList(viewport));
            // for (int layer_i = 0; layer_i < IM_ARRAYSIZE(viewport->DrawDataBuilder.Layers); layer_i++)
            //     for (int draw_list_i = 0; draw_list_i < viewport->DrawDataBuilder.Layers[layer_i].Size; draw_list_i++) {
            //         if (std::string(viewport->DrawDataBuilder.Layers[layer_i][draw_list_i]->_OwnerName) == "Viewport") {
            //             ImDrawList* _draw_list = viewport->DrawDataBuilder.Layers[layer_i][draw_list_i];
            //             DebugNodeDrawList(viewport, _draw_list);
            //         }
            //     }
        }
    }
}

}