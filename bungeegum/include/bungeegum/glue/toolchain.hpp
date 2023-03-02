//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <cstdint>

// arch detection
#if INTPTR_MAX == INT32_MAX
#define TOOLCHAIN_ARCH_32 1
#define TOOLCHAIN_ARCH_64 0
#elif INTPTR_MAX == INT64_MAX
#define TOOLCHAIN_ARCH_32 0
#define TOOLCHAIN_ARCH_64 1
#else
#error BAD IMPLEMENTATION (unknown pointer size or missing size macros)
#endif

// cpu detection
#if defined(__EMSCRIPTEN__)
#define TOOLCHAIN_CPU_X86 0
#define TOOLCHAIN_CPU_ARM 0
#define TOOLCHAIN_CPU_WEBASM 1
#elif defined(__x86_64__) || defined(_M_X64) || defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) // x64
#define TOOLCHAIN_CPU_X86 1
#define TOOLCHAIN_CPU_ARM 0
#define TOOLCHAIN_CPU_WEBASM 0
#elif defined(__arm__) || defined(__aarch64__) || defined(_ARM) || defined(_M_ARM) || defined(__arm)
#define TOOLCHAIN_CPU_X86 0
#define TOOLCHAIN_CPU_ARM 1
#define TOOLCHAIN_CPU_WEBASM 0
#else
#define TOOLCHAIN_CPU_X86 0
#define TOOLCHAIN_CPU_ARM 0
#define TOOLCHAIN_CPU_WEBASM 0
#endif

// platform detection
#if defined(__EMSCRIPTEN__)
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 1
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#elif defined(__android__)
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 1
#elif defined(__linux__)
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 1
#define TOOLCHAIN_PLATFORM_ANDROID 0
#elif defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 1
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#elif TARGET_OS_MAC
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 1
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#else
#error "BAD IMPLEMENTATION (unknown Apple platform)"
#endif
#elif defined(_WIN32)
#include <winapifamily.h>
#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 1
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#else
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 1
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#endif
#else
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#endif

// compiler detection
#if defined(_MSC_VER)
#define TOOLCHAIN_COMPILER_MSVC 1
#define TOOLCHAIN_COMPILER_CLANG 0
#define TOOLCHAIN_COMPILER_GCC 0
#elif defined(__clang__)
#define TOOLCHAIN_COMPILER_MSVC 0
#define TOOLCHAIN_COMPILER_CLANG 1
#define TOOLCHAIN_COMPILER_GCC 0
#elif defined(__gcc__)
#define TOOLCHAIN_COMPILER_MSVC 0
#define TOOLCHAIN_COMPILER_CLANG 0
#define TOOLCHAIN_COMPILER_GCC 1
#else
#define TOOLCHAIN_COMPILER_MSVC 0
#define TOOLCHAIN_COMPILER_CLANG 0
#define TOOLCHAIN_COMPILER_GCC 0
#endif

// build variant
#if defined(NDEBUG)
#define TOOLCHAIN_BUILD_DEBUG 0
#define TOOLCHAIN_BUILD_RELEASE 1
#else
#define TOOLCHAIN_BUILD_DEBUG 1
#define TOOLCHAIN_BUILD_RELEASE 0
#endif

namespace bungeegum {
namespace detail {

    constexpr bool is_arch_32_v = TOOLCHAIN_ARCH_32;
    constexpr bool is_arch_64_v = TOOLCHAIN_ARCH_64;

    constexpr bool is_cpu_x86_v = TOOLCHAIN_CPU_X86;
    constexpr bool is_cpu_arm_v = TOOLCHAIN_CPU_ARM;
    constexpr bool is_cpu_webasm_v = TOOLCHAIN_CPU_WEBASM;

    constexpr bool is_platform_emscripten_v = TOOLCHAIN_PLATFORM_EMSCRIPTEN;
    constexpr bool is_platform_win32_v = TOOLCHAIN_PLATFORM_WIN32;
    constexpr bool is_platform_uwp_v = TOOLCHAIN_PLATFORM_UWP;
    constexpr bool is_platform_macos_v = TOOLCHAIN_PLATFORM_MACOS;
    constexpr bool is_platform_ios_v = TOOLCHAIN_PLATFORM_IOS;
    constexpr bool is_platform_linux_v = TOOLCHAIN_PLATFORM_LINUX;
    constexpr bool is_platform_android_v = TOOLCHAIN_PLATFORM_ANDROID;

    constexpr bool is_compiler_msvc_v = TOOLCHAIN_COMPILER_MSVC;
    constexpr bool is_compiler_clang_v = TOOLCHAIN_COMPILER_CLANG;
    constexpr bool is_compiler_gcc_v = TOOLCHAIN_COMPILER_GCC;

    constexpr bool is_build_debug_v = TOOLCHAIN_BUILD_DEBUG;
    constexpr bool is_build_release_v = TOOLCHAIN_BUILD_RELEASE;

}
}