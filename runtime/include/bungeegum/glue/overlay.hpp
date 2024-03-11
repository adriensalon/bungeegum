#pragma once

#include <bungeegum/config/feature.hpp>
#include <bungeegum/glue/rendering.hpp>

namespace bungeegum { 
namespace detail { 

    /// @brief
    struct overlay_handle {
        overlay_handle() = default;
        overlay_handle(const overlay_handle& other) = default;
        overlay_handle& operator=(const overlay_handle& other) = default;
        overlay_handle(overlay_handle&& other) = default;
        overlay_handle& operator=(overlay_handle&& other) = default;

        /// @brief
        void emplace(rasterizer_handle& context);

        /// @brief
        bool has_value() const;

        /// @brief
        void draw() const;

        /// @brief
        void reset();        
        
        /// @brief
        bool show_build = false;
        
        /// @brief
        bool show_hierarchy = false;
        
        /// @brief
        bool show_hotswap = false;
        
        /// @brief
        bool show_output = false;
        
        /// @brief
        bool show_profiling = false;

    private:
        bool _has_value = false;
        font_handle _regular_font = {};
        font_handle _bold_font = {};
        std::size_t _vertices_count = 0u;
        std::size_t _indices_count = 0u;
        std::size_t _commands_count = 0u;
    };

}
}