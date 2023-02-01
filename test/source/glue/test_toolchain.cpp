//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <gtest/gtest.h>
#include <staticgui/glue/toolchain.hpp>

TEST(staticgui_glue, toolchain_arch)
{
    unsigned int _sum = 0
        + staticgui::glue::is_arch_32
        + staticgui::glue::is_arch_64;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(staticgui_glue, toolchain_cpu)
{
    unsigned int _sum = 0
        + staticgui::glue::is_cpu_x86
        + staticgui::glue::is_cpu_arm
        + staticgui::glue::is_cpu_webasm;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(staticgui_glue, toolchain_platform)
{
    unsigned int _sum = 0
        + staticgui::glue::is_platform_emscripten
        + staticgui::glue::is_platform_win32
        + staticgui::glue::is_platform_uwp
        + staticgui::glue::is_platform_macos
        + staticgui::glue::is_platform_ios
        + staticgui::glue::is_platform_linux
        + staticgui::glue::is_platform_android;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(staticgui_glue, toolchain_compiler)
{
    unsigned int _sum = 0
        + staticgui::glue::is_compiler_msvc
        + staticgui::glue::is_compiler_clang
        + staticgui::glue::is_compiler_gcc;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}