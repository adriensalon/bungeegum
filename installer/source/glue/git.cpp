#pragma once

#include <process.hpp>

#include <bungeegum/glue/console.hpp>
#include <bungeegum/glue/git.hpp>

namespace bungeegum {
namespace detail {

    std::optional<std::string> git_version()
    {
        std::optional<std::string> _git_version = std::nullopt;
        TinyProcessLib::Process _git_process(
            "git --version", "",
            [&_git_version](const char* bytes, size_t n) {
                std::string _line_str(bytes, n);
                std::size_t _offset = _line_str.find_last_of(' ');
                if (_offset != _line_str.npos) {
                    _offset++;
                }
                std::size_t _length = _line_str.length() - _offset;
                _git_version = _line_str.substr(_offset, _length - 1);
            });
        int _exit_status = _git_process.get_exit_status();
        if (_exit_status != 0) {
            return std::nullopt;
        }
        return _git_version;
    }

    void git_clone(const std::string& url, const std::filesystem::path& install_directory, const std::string& branch)
    {
        TinyProcessLib::Process _git_process(
            "git clone " + url + " --branch " + branch + " --single-branch", install_directory.generic_string(),
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
        int _exit_status = _git_process.get_exit_status();
    }

    // bool is_tag_newer(const std::string& found_tag, const std::string& new_tag);
}
}