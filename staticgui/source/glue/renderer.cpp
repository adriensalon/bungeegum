//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/renderer.hpp>

namespace staticgui {
namespace glue {

    renderer::renderer(const window& existing)
    {
        if constexpr (is_renderer_backend_directx) {

        } else if constexpr (is_renderer_backend_vulkan) {

        } else if constexpr (is_renderer_backend_opengl) {
        }
    }

    renderer::~renderer()
    {
    }

    renderer::renderer(renderer&& other)
    {
    }

    renderer& renderer::operator=(renderer&& other)
    {
        return *this;
    }

    void* renderer::get_native_device() const
    {
        return nullptr;
    }

    void* renderer::get_native_swap_chain() const
    {
        return nullptr;
    }

    void* renderer::get_native_context() const
    {
        return nullptr;
    }

}
}