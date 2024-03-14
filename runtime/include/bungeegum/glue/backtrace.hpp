#pragma once

#include <bungeegum/config/config.hpp>

#include <cstddef>
#include <exception>
#include <filesystem>
#include <string>
#include <vector>
#include <functional>
#include <optional>

namespace bungeegum {
namespace detail {

    /// @brief Instance of this struct store data from a backtraced call.
    struct backtraced_step {
        
        /// @brief
        void* address;

        /// @brief
        std::filesystem::path file;
        
        /// @brief
        std::string function;
        
        /// @brief
        std::size_t line;
        
        /// @brief
        std::size_t column;
    };

    /// @brief Instances of this struct can be thrown as exceptions. They hold a string error
    /// message and trace the stack when constructed.
    /// @details Instances of this struct can only be moved.
    struct backtraced_exception : public std::exception {
        backtraced_exception() = delete;
        backtraced_exception(const backtraced_exception& other) = delete;
        backtraced_exception& operator=(const backtraced_exception& other) = delete;
        backtraced_exception(backtraced_exception&& other) = default;
        backtraced_exception& operator=(backtraced_exception&& other) = default;

        /// @brief Creates an instance from an error message and the count of calls to backtrace.
        backtraced_exception(const std::string& tag, const std::string& what, const std::size_t tracing_offset = 0u, const std::size_t tracing_size = BUNGEEGUM_USE_BACKTRACE_SIZE);

        /// @brief Creates an instance from an error message and the count of calls to backtrace.
        backtraced_exception(const std::string& tag, const std::wstring& what, const std::size_t tracing_offset = 0u, const std::size_t tracing_size = BUNGEEGUM_USE_BACKTRACE_SIZE);

        /// @brief Creates an instance from an existing exception and the count of calls to
        /// backtrace.
        backtraced_exception(const std::string& tag, const std::exception& existing, const std::size_t tracing_offset = 0u, const std::size_t tracing_size = BUNGEEGUM_USE_BACKTRACE_SIZE);

        /// @brief Gets the error message.
        [[nodiscard]] const char* tag() const noexcept;

        /// @brief Gets the error message.
        [[nodiscard]] const char* what() const noexcept;
        
#if BUNGEEGUM_USE_OVERLAY

        /// @brief Gets the error message.
        [[nodiscard]] const char* key() const noexcept;

#endif

        /// @brief Data resulting from tracing.
        std::vector<backtraced_step> tracing;

    private:
        const std::string _tag;
        const std::string _what;
#if BUNGEEGUM_USE_OVERLAY
        const std::string _key;
#endif
    };

	/// @brief 
	/// @param try_callback 
	/// @param catch_callback 
	void protect(
		const std::function<void()>& try_callback,
		const std::function<void(backtraced_exception&&)>& catch_callback = nullptr);

}
}
