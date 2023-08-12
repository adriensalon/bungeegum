#include <gtest/gtest.h>

#include <bungeegum/glue/toolchain.hpp>

TEST(bungeegum_glue, toolchain_arch)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_arch_32
        + bungeegum::detail::is_arch_64;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(bungeegum_glue, toolchain_cpu)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_cpu_x86
        + bungeegum::detail::is_cpu_arm
        + bungeegum::detail::is_cpu_webasm;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(bungeegum_glue, toolchain_platform)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_platform_emscripten
        + bungeegum::detail::is_platform_win32
        + bungeegum::detail::is_platform_uwp
        + bungeegum::detail::is_platform_macos
        + bungeegum::detail::is_platform_ios
        + bungeegum::detail::is_platform_linux
        + bungeegum::detail::is_platform_android;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}

TEST(bungeegum_glue, toolchain_compiler)
{
    unsigned int _sum = 0
        + bungeegum::detail::is_compiler_msvc
        + bungeegum::detail::is_compiler_clang
        + bungeegum::detail::is_compiler_gcc;
    bool _defined = (_sum == 1);
    EXPECT_TRUE(_defined);
}