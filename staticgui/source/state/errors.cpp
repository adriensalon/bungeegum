//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <imgui.h>

#include <staticgui/glue/backtrace.hpp>
#include <staticgui/glue/console.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    struct library_bad_usage_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    struct library_bad_implementation_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    void throw_library_bad_usage(const std::string& what)
    {
        throw library_bad_usage_exception(what, 10);
    }

    void throw_library_bad_implementation(const std::string& what)
    {
        throw library_bad_implementation_exception(what, 10);
    }

    void protect_library(const std::function<void()>& try_callback)
    {
        glue::try_catch_nullptr(
            [&]() {
                try {
                    try_callback();
                } catch (library_bad_implementation_exception& _exception) {
                    std::cout << "BAD IMPL 2 \n";
                    // bad implementation exception (traced)
                    for (auto& _trace : _exception.tracing)
                        glue::console_log(_trace.primary.function, glue::console_color::green);
                } catch (const std::exception& _exception) {
                    std::cout << "UNKNOWN 2\n";
                    (void)_exception;
                    // bad implementation exception (non traced)
                }
            },
            [&]() {
                // nullptr exception
            });
    }

    static bool has_userspace_thrown_memory = false;

    struct userspace_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    void throw_userspace(const std::string& what)
    {
        throw userspace_exception(what, 10);
    }

    void protect_userspace(const std::function<void()>& try_callback)
    {
        if (!has_userspace_thrown_memory) {
            try {
                try_callback();
            } catch (library_bad_implementation_exception&) {
                throw;
            } catch (...) { // detail here
                has_userspace_thrown_memory = true;
            }
        }
    }

    bool has_userspace_thrown()
    {
        return has_userspace_thrown_memory;
    }

    void display_userspace_gui()
    {
        ImGui::ShowDemoWindow();
    }
}
}
