//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <bungeegum/glue/toolchain.hpp>
#include <gtest/gtest.h>


TEST(bungeegum_glue, toolchain_arch)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_arch_32_v
        + bungeegum::detail::is_arch_64_v;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(bungeegum_glue, toolchain_cpu)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_cpu_x86_v
        + bungeegum::detail::is_cpu_arm_v
        + bungeegum::detail::is_cpu_webasm_v;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(bungeegum_glue, toolchain_platform)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_platform_emscripten_v
        + bungeegum::detail::is_platform_win32_v
        + bungeegum::detail::is_platform_uwp_v
        + bungeegum::detail::is_platform_macos_v
        + bungeegum::detail::is_platform_ios_v
        + bungeegum::detail::is_platform_linux_v
        + bungeegum::detail::is_platform_android_v;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(bungeegum_glue, toolchain_compiler)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_compiler_msvc_v
        + bungeegum::detail::is_compiler_clang_v
        + bungeegum::detail::is_compiler_gcc_v;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}