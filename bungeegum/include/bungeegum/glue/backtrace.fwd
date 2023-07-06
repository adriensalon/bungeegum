#pragma once

#include <cstddef>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>

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
    struct backtraced_exception : std::exception {
        backtraced_exception() = delete;
        backtraced_exception(const backtraced_exception& other) = delete;
        backtraced_exception& operator=(const backtraced_exception& other) = delete;
        backtraced_exception(backtraced_exception&& other);
        backtraced_exception& operator=(backtraced_exception&& other);

        /// @brief Creates an instance from an error message and the count of calls to backtrace.
        backtraced_exception(const std::string& what, const std::size_t tracing_size);

        /// @brief Creates an instance from an existing exception and the count of calls to
        /// backtrace.
        backtraced_exception(const std::exception& existing, const std::size_t tracing_size);

        /// @brief Gets the error message.
        [[nodiscard]] const char* what() const;

        /// @brief Data resulting from tracing.
        std::vector<backtraced_result> tracing;

    private:
        std::string _what;
    };
}
}
