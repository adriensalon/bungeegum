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
#error Unknown pointer size or missing size macros. Please define INTPTR_MAX && (INT32_MAX || INT64_MAX).
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
#define TOOLCHAIN_PLATFORM_NAME "emscripten"
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 1
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#elif defined(__android__)
#define TOOLCHAIN_PLATFORM_NAME "android"
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 1
#elif defined(__linux__)
#define TOOLCHAIN_PLATFORM_NAME "linux"
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
#define TOOLCHAIN_PLATFORM_NAME "ios"
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 1
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#elif TARGET_OS_MAC
#define TOOLCHAIN_PLATFORM_NAME "macos"
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 1
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#else
#error "Undefined Apple platform. Please define TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || TARGET_OS_MAC."
#endif
#elif defined(_WIN32)
#include <winapifamily.h>
#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#define TOOLCHAIN_PLATFORM_NAME "win32"
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_WIN32 1
#define TOOLCHAIN_PLATFORM_UWP 0
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#else
#define TOOLCHAIN_PLATFORM_EMSCRIPTEN 0
#define TOOLCHAIN_PLATFORM_NAME "uwp"
#define TOOLCHAIN_PLATFORM_WIN32 0
#define TOOLCHAIN_PLATFORM_UWP 1
#define TOOLCHAIN_PLATFORM_MACOS 0
#define TOOLCHAIN_PLATFORM_IOS 0
#define TOOLCHAIN_PLATFORM_LINUX 0
#define TOOLCHAIN_PLATFORM_ANDROID 0
#endif
#else
#define TOOLCHAIN_PLATFORM_NAME "undefined"
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

// build variant detection
#if defined(NDEBUG)
#define TOOLCHAIN_BUILD_DEBUG 0
#define TOOLCHAIN_BUILD_RELEASE 1
#else
#define TOOLCHAIN_BUILD_DEBUG 1
#define TOOLCHAIN_BUILD_RELEASE 0
#endif

namespace bungeegum {
namespace detail {

    /// @brief True if the current architecture is 32bits.
    constexpr bool is_arch_32 = TOOLCHAIN_ARCH_32;

    /// @brief True if the current architecture is 64bits.
    constexpr bool is_arch_64 = TOOLCHAIN_ARCH_64;

    /// @brief True if the current CPU architecture is x86.
    constexpr bool is_cpu_x86 = TOOLCHAIN_CPU_X86;

    /// @brief True if the current CPU architecture is ARM.
    constexpr bool is_cpu_arm = TOOLCHAIN_CPU_ARM;

    /// @brief True if the current "CPU" architecture is WEBASM.
    constexpr bool is_cpu_webasm = TOOLCHAIN_CPU_WEBASM;

    /// @brief True if the current platform is emscripten.
    constexpr bool is_platform_emscripten = TOOLCHAIN_PLATFORM_EMSCRIPTEN;

    /// @brief True if the current platform is Win32.
    constexpr bool is_platform_win32 = TOOLCHAIN_PLATFORM_WIN32;

    /// @brief True if the current platform is Universal Windows Platform.
    constexpr bool is_platform_uwp = TOOLCHAIN_PLATFORM_UWP;

    /// @brief True if the current platform is MacOS.
    constexpr bool is_platform_macos = TOOLCHAIN_PLATFORM_MACOS;

    /// @brief True if the current platform is iOS.
    constexpr bool is_platform_ios = TOOLCHAIN_PLATFORM_IOS;

    /// @brief True if the current platform is Linux but not Android.
    constexpr bool is_platform_linux = TOOLCHAIN_PLATFORM_LINUX;

    /// @brief True if the current platform is Android.
    constexpr bool is_platform_android = TOOLCHAIN_PLATFORM_ANDROID;

    /// @brief True if the current compiler is MSVC.
    constexpr bool is_compiler_msvc = TOOLCHAIN_COMPILER_MSVC;

    /// @brief True if the current compiler is Clang.
    constexpr bool is_compiler_clang = TOOLCHAIN_COMPILER_CLANG;

    /// @brief True if the current compiler is GCC.
    constexpr bool is_compiler_gcc = TOOLCHAIN_COMPILER_GCC;

    /// @brief True if the current build variant is debug.
    constexpr bool is_build_debug = TOOLCHAIN_BUILD_DEBUG;

    /// @brief True if the current build variant is release.
    constexpr bool is_build_release = TOOLCHAIN_BUILD_RELEASE;
}
}