//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

struct ImFont;

#include <staticgui/glue/simd.hpp>

namespace staticgui {

struct font_guard {
    font_guard();
    font_guard(const font_guard& other) = delete;
    font_guard(ImFont* font);
    ~font_guard();

    void set(ImFont* font);
    void release();

private:
    bool _is_set = false;
};

struct item_guard {
    item_guard();
    item_guard(const item_guard& other) = delete;
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
    style_guard(const int& target, const float value);
    style_guard(const int& target, const float2 value);
    ~style_guard();

    void set(const int& target, const float value);
    void set(const int& target, const float2 value);
    void release();

private:
    bool _is_set = false;
};

}
