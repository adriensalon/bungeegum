#pragma once

#include <bungeegum/core/log.fwd>

namespace bungeegum {

/// @brief
enum struct log_level {

    /// @brief and exits immediately the current function.
    error,

    /// @brief
    warning,

    /// @brief
    message
};

/// @brief
void log_error(const std::string& what);

/// @brief
void log_warning(const std::string& what);

/// @brief
void log_message(const std::string& what);
}