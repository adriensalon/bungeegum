#pragma once

#include <string>

#include <bungeegum/core/math.hpp>

struct ImFont;

namespace bungeegum {
namespace detail {

    /// @brief 
    struct font_guard {
        font_guard();
        font_guard(const font_guard& other) = delete;
        font_guard& operator=(const font_guard& other) = delete;
        font_guard(font_guard&& other) = default;
        font_guard& operator=(font_guard&& other) = default;
        font_guard(ImFont* font);
        ~font_guard();

        /// @brief 
        /// @param font 
        void set(ImFont* font);

        /// @brief 
        void release();

    private:
        bool _is_set = false;
    };

    struct item_guard {
        item_guard();
        item_guard(const item_guard& other) = delete;
        item_guard& operator=(const item_guard& other) = delete;
        item_guard(item_guard&& other) = default;
        item_guard& operator=(item_guard&& other) = default;
        item_guard(const int target, const bool value);
        ~item_guard();

        void set(const int target, const bool value);
        void release();

    private:
        bool _is_set = false;
    };

    struct color_guard {
        color_guard();
        color_guard(const color_guard& other) = delete;
        color_guard& operator=(const color_guard& other) = delete;
        color_guard(color_guard&& other) = default;
        color_guard& operator=(color_guard&& other) = default;
        color_guard(const int& target, const float4 value);
        ~color_guard();

        void set(const int& target, const float4 value);
        void release();

    private:
        bool _is_set = false;
    };

    struct style_guard {
        style_guard();
        style_guard(const style_guard& other) = delete;
        style_guard& operator=(const style_guard& other) = delete;
        style_guard(style_guard&& other) = default;
        style_guard& operator=(style_guard&& other) = default;
        style_guard(const int& target, const float value);
        style_guard(const int& target, const float2 value);
        ~style_guard();

        void set(const int& target, const float value);
        void set(const int& target, const float2 value);
        void release();

    private:
        bool _is_set = false;
    };

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value);
}
}

#include <bungeegum/glue/theme.inl>