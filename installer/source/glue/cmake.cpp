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
        const std::optional<std::string>& generator,
        const std::vector<std::string>& definitions)
    {
    }

    // void cmake_build(const std::filesystem::path& build_directory);
}
}