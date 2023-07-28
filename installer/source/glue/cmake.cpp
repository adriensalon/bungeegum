#pragma once

#include <process.hpp>

#include <fstream>

#include <bungeegum/glue/cmake.hpp>
#include <bungeegum/glue/console.hpp>

namespace bungeegum {
namespace detail {

    std::optional<std::string> cmake_version()
    {
        std::optional<std::string> _cmake_version = std::nullopt;
        TinyProcessLib::Process _cmake_process(
            "cmake --version", "",
            [&_cmake_version](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                if (_line_str.substr(0, 13) == "cmake version") {
                    std::size_t _line_end = _line_str.find_first_of('\n');
                    std::size_t _offset = 14;
                    std::size_t _length = _line_end - _offset;
                    _cmake_version = _line_str.substr(_offset, _length);
                }
            });
        int _exit_status = _cmake_process.get_exit_status();
        if (_exit_status != 0) {
            return std::nullopt;
        }
        return _cmake_version;
    }

    void cmake_configure(
        const std::filesystem::path& source_directory,
        const std::filesystem::path& build_directory,
        const std::optional<std::filesystem::path> install_directory,
        const std::optional<std::string>& generator,
        const std::vector<std::pair<std::string, bool>>& definitions,
        const bool debug)
    {
        std::string _cmake_string = "cmake";
        _cmake_string += " -S " + source_directory.generic_string();
        _cmake_string += " -B " + build_directory.generic_string();
        if (generator.has_value()) {
            _cmake_string += " -G " + generator.value();
        }
        for (const std::pair<std::string, bool> _definition : definitions) {
            _cmake_string += " -D" + _definition.first + "=" + (_definition.second ? "ON" : "OFF");
        }
        if (debug) {
            _cmake_string += " -DCMAKE_BUILD_TYPE=Debug";
        } else {
            _cmake_string += " -DCMAKE_BUILD_TYPE=Release";
        }
        if (install_directory.has_value()) {
            _cmake_string += " -DBUNGEEGUM_INSTALL_DIR=\"" + install_directory.value().generic_string() + "\"";
        }
        TinyProcessLib::Process _cmake_process(
            _cmake_string, "",
            [](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                detail::console_log(_line_str, detail::console_color::blue);
                std::cout << std::flush;
            },
            [](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                detail::console_log(_line_str, detail::console_color::blue);
                std::cout << std::flush;
            });
        int _exit_status = _cmake_process.get_exit_status();
    }

    void cmake_build(const std::filesystem::path& build_directory, const std::optional<std::string>& target)
    {
        std::string _cmake_string = "cmake --build " + build_directory.generic_string();
        if (target.has_value()) {
            _cmake_string += " --target " + target.value();
        }
        TinyProcessLib::Process _cmake_process(
            _cmake_string, "",
            [](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                detail::console_log(_line_str, detail::console_color::blue);
                std::cout << std::flush;
            },
            [](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                detail::console_log(_line_str, detail::console_color::blue);
                std::cout << std::flush;
            });
        int _exit_status = _cmake_process.get_exit_status();
    }

    void cmake_install(const std::filesystem::path& build_directory, const std::filesystem::path& prefix, const std::optional<std::string>& target)
    {
        std::string _cmake_string = "cmake --install " + build_directory.generic_string() + " --prefix \"" + prefix.generic_string() + "\"";
        if (target.has_value()) {
            _cmake_string += " --target " + target.value();
        }
        TinyProcessLib::Process _cmake_process(
            _cmake_string, "",
            [](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                detail::console_log(_line_str, detail::console_color::blue);
                std::cout << std::flush;
            },
            [](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                detail::console_log(_line_str, detail::console_color::blue);
                std::cout << std::flush;
            });
        int _exit_status = _cmake_process.get_exit_status();
    }
}
}