//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <functional>

namespace staticgui {
namespace internal {
    namespace app {

        enum struct backend {
            automatic,
            opengl,
            directx,
            vulkan
        };

        void launch_app(
            void* existing_window = nullptr,
            const backend prefered = backend::automatic,
            std::function<void()> on_create_callback = nullptr,
            std::function<void()> on_update_callback = nullptr,
            std::function<void()> on_destroy_callback = nullptr);

        struct attachment {
            std::function<void()> on_create_callback;
            std::function<void()> on_update_callback;
            std::function<void()> on_destroy_callback;
        };

        attachment attach_app(
            void* existing_window,
            std::function<void()> on_create_callback,
            std::function<void()> on_update_callback,
            std::function<void()> on_destroy_callback);
    }
}
}