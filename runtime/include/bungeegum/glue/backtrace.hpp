#pragma once

#include <cstddef>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

#include <bungeegum/glue/config.hpp>
#include <bungeegum/glue/toolchain.hpp>

/// @brief Defines if we use backtraced exceptions.
#if !defined(BUNGEEGUM_USE_BACKTRACE)
#define BUNGEEGUM_USE_BACKTRACE (BUNGEEGUM_ENABLE_BACKTRACE && !TOOLCHAIN_PLATFORM_EMSCRIPTEN)
#endif

/// @brief Defines the count of traces to produce when creating backtraced exceptions.
#if !defined(BUNGEEGUM_USE_BACKTRACE_SIZE)
#define BUNGEEGUM_USE_BACKTRACE_SIZE 10
#endif

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct store additional data from a backtraced call.
    struct backtraced_source {
        std::filesystem::path file;
        std::string function;
        std::size_t line;
        std::size_t column;
    };

    /// @brief Instance of this struct store data from a backtraced call.
    struct backtraced_result {
        void* address;
        backtraced_source primary;
        std::vector<backtraced_source> inliners;
    };

    /// @brief Instances of this struct can be thrown as exceptions. They hold a string error
    /// message and trace the stack when constructed.
    /// @details Instances of this struct can only be moved.
    struct backtraced_exception : public std::exception {
        backtraced_exception() = delete;
        backtraced_exception(const backtraced_exception& other) = delete;
        backtraced_exception& operator=(const backtraced_exception& other) = delete;
        backtraced_exception(backtraced_exception&& other);
        backtraced_exception& operator=(backtraced_exception&& other);

        /// @brief Creates an instance from an error message and the count of calls to backtrace.
        backtraced_exception(const std::string& what, const std::size_t tracing_offset = 0u, const std::size_t tracing_size = BUNGEEGUM_USE_BACKTRACE_SIZE);

        /// @brief Creates an instance from an error message and the count of calls to backtrace.
        backtraced_exception(const std::wstring& what, const std::size_t tracing_offset = 0u, const std::size_t tracing_size = BUNGEEGUM_USE_BACKTRACE_SIZE);

        /// @brief Creates an instance from an existing exception and the count of calls to
        /// backtrace.
        backtraced_exception(const std::exception& existing, const std::size_t tracing_offset = 0u, const std::size_t tracing_size = BUNGEEGUM_USE_BACKTRACE_SIZE);

        /// @brief Gets the error message.
        [[nodiscard]] const char* what() const;

        /// @brief Data resulting from tracing.
        std::vector<backtraced_result> tracing;

    private:
        std::string _what;
    };
}
}
