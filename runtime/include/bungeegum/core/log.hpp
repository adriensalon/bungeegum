#pragma once

#include <bungeegum/core/log.fwd>

namespace bungeegum {

/// @brief Logs an error and exits userspace without crashing the program.
/// @details If BUNGEEGUM_ENABLE_OVERLAY equals 1 the error will be sent to the overlay, otherwise
/// it will be printed to the OS console. If BUNGEEGUM_ENABLE_BACKTRACE equals 1 the error will
/// include stack trace information.
void log_error(const std::string& what);

/// @brief Logs an error and exits userspace without crashing the program.
/// @details If BUNGEEGUM_ENABLE_OVERLAY equals 1 the error will be sent to the overlay, otherwise
/// it will be printed to the OS console. If BUNGEEGUM_ENABLE_BACKTRACE equals 1 the error will
/// include stack trace information.
void log_error(const std::wstring& what);

/// @brief Logs a warning and exits userspace without crashing the program.
/// @details If BUNGEEGUM_ENABLE_OVERLAY equals 1 the warning will be sent to the overlay,
/// otherwise it will be printed to the OS console. If BUNGEEGUM_ENABLE_BACKTRACE equals 1 the
/// warning will include stack trace information.
void log_warning(const std::string& what);

/// @brief Logs a warning and exits userspace without crashing the program.
/// @details If BUNGEEGUM_ENABLE_OVERLAY equals 1 the warning will be sent to the overlay,
/// otherwise it will be printed to the OS console. If BUNGEEGUM_ENABLE_BACKTRACE equals 1 the
/// warning will include stack trace information.
void log_warning(const std::wstring& what);

/// @brief Logs a message and exits userspace without crashing the program.
/// @details If BUNGEEGUM_ENABLE_OVERLAY equals 1 the message will be sent to the overlay,
/// otherwise it will be printed to the OS console. If BUNGEEGUM_ENABLE_BACKTRACE equals 1 the
/// message will include stack trace information.
void log_message(const std::string& what);

/// @brief Logs a message and exits userspace without crashing the program.
/// @details If BUNGEEGUM_ENABLE_OVERLAY equals 1 the message will be sent to the overlay,
/// otherwise it will be printed to the OS console. If BUNGEEGUM_ENABLE_BACKTRACE equals 1 the
/// message will include stack trace information.
void log_message(const std::wstring& what);
}