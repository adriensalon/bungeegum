#pragma once

#include <process.hpp>

#include <fstream>

#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/doxygen.hpp>

namespace bungeegum {
namespace detail {

    std::optional<std::string> doxygen_version()
    {
        std::optional<std::string> _doxygen_version = std::nullopt;
        TinyProcessLib::Process _doxygen_process(
            "doxygen --version", "",
            [&_doxygen_version](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                std::size_t _length = _line_str.find_first_of(' ');
                _doxygen_version = _line_str.substr(0, _length);
            });
        int _exit_status = _doxygen_process.get_exit_status();
        if (_exit_status != 0) {
            return std::nullopt;
        }
        return _doxygen_version;
    }
}
}