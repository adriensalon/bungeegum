//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <staticgui/glue/backtrace.hpp>
#include <staticgui/glue/console.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    struct userspace_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    struct bad_usage_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    struct bad_implementation_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    void log_nullptr()
    {
    }

    void log_bad_implementation()
    {
    }

    void throw_userspace(const std::string& what)
    {
    }

    void throw_bad_usage(const std::string& what)
    {
    }

    void throw_bad_implementation(const std::string& what)
    {
        throw bad_implementation_exception(what, 10);
    }

    void try_catch_user_space_strategy(const std::function<void()>& try_callback, const std::function<void()>& catch_callback)
    {
        try_callback();
    }

    void try_catch_user_space_island(const std::function<void()>& try_callback)
    {
        // glue::try_catch_nullptr(
        //     [&]() {
        try {
            try_callback();
            // } catch< (bad_usage_exception& _exception) {
            //     if (catch_callback)
            //         catch_callback();
            //     if (rethrow)
            //         throw;
            //     else {
            //         // bad usage exception (traced)
            // (void)_exception;
            // }

        } catch (userspace_exception& _exception) {
            std::cout << "USERSPACE \n";
            (void)_exception;
            // userspace exception(traced)
        } catch (bad_implementation_exception& _exception) {
            std::cout << "BAD IMPL 1 \n";
            // if (catch_callback)
            //     catch_callback();
            // if (rethrow)
            //     throw;
            // else {
            //     /// bad implementation exception (traced)
            for (auto& _trace : _exception.tracing)
                glue::console_log(_trace.primary.function, glue::console_color::blue);
            throw;
        }
        // catch (const std::exception& _exception) {
        //     std::cout << "UNKNOWN \n";
        //     (void)_exception;
        //     // userspace exception (non traced)
        // }
        // },
        // [&]() {
        //     // nullptr exception
        // });
    }

    void try_catch_library_space(const std::function<void()>& try_callback)
    {
        glue::try_catch_nullptr(
            [&]() {
                try {
                    try_callback();
                } catch (bad_implementation_exception& _exception) {
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

    void display_user_space_gui()
    {
    }

}
}
