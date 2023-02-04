#pragma once

#include <memory>
#include "ImGuiImplDiligent.hpp"

struct SDL_Window;

namespace Diligent
{

class ImGuiImplSDL final : public ImGuiImplDiligent
{
public:
    ImGuiImplSDL(SDL_Window*    window,
                 IRenderDevice* pDevice,
                 TEXTURE_FORMAT BackBufferFmt,
                 TEXTURE_FORMAT DepthBufferFmt,
                 Uint32         InitialVertexBufferSize = ImGuiImplDiligent::DefaultInitialVBSize,
                 Uint32         InitialIndexBufferSize  = ImGuiImplDiligent::DefaultInitialIBSize);
    ~ImGuiImplSDL();

    // clang-format off
    ImGuiImplSDL             (const ImGuiImplSDL&)  = delete;
    ImGuiImplSDL             (      ImGuiImplSDL&&) = delete;
    ImGuiImplSDL& operator = (const ImGuiImplSDL&)  = delete;
    ImGuiImplSDL& operator = (      ImGuiImplSDL&&) = delete;
    // clang-format on

    virtual void NewFrame(Uint32 RenderSurfaceWidth, Uint32 RenderSurfaceHeight, SURFACE_TRANSFORM SurfacePreTransform) override final;
};

} // namespace Diligent
