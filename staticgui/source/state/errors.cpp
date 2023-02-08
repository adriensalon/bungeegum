//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <optional>

#include <imgui.h>

#include <staticgui/glue/backtrace.hpp>
#include <staticgui/glue/console.hpp>
#include <staticgui/state/errors.hpp>
#include <staticgui/state/widget.hpp>

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
        // if not protecting => console
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
                    std::cout << _exception.what() << std::endl;
                    (void)_exception;
                    // bad implementation exception (non traced)
                }
            },
            [&]() {
                // nullptr exception
            });
    }

    // struct userspace_thrown_exception_data {
    //     std::string what;
    //     std::vector<glue::backtraced_result> tracing;
    // };

    static std::optional<glue::backtraced_exception> userspace_thrown_exception = std::nullopt;
    static bool has_userspace_thrown_memory = false;

    struct user_bad_implementation_exception : public glue::backtraced_exception {
        using glue::backtraced_exception::backtraced_exception;
    };

    void throw_user_bad_implementation(const std::string& what)
    {
        throw user_bad_implementation_exception(what, 10);
    }

    void protect_userspace(const std::function<void()>& try_callback)
    {
        if (!has_userspace_thrown_memory) {
            try {
                try_callback();
            } catch (library_bad_implementation_exception&) {
                throw;
            } catch (library_bad_usage_exception& _exception) {
                userspace_thrown_exception = std::move(_exception);
                has_userspace_thrown_memory = true;
            } catch (user_bad_implementation_exception& _exception) {
                userspace_thrown_exception = std::move(_exception);
                has_userspace_thrown_memory = true;
            } catch (std::exception& _exception) {
                has_userspace_thrown_memory = true;
                (void)_exception;
                // TODO
            } catch (...) { // detail here
                has_userspace_thrown_memory = true;
                // TODO
            }
        }
    }

    bool has_userspace_thrown()
    {
        return has_userspace_thrown_memory;
    }

    std::optional<glue::backtraced_exception>& get_userspace_thrown_exception()
    {
        return userspace_thrown_exception;
    }

    // void display_userspace_gui()
    // {
    //     ImGui::StyleColorsLight();
    //     if (ImGui::Begin("Exception caught")) {
    //         ImGui::Text(userspace_thrown_exception.value().what());
    //         for (auto& _trace : userspace_thrown_exception.value().tracing) {

    //             ImGui::TextColored(ImVec4(0.1f, 0.1f, 0.8f, 1.f), _trace.primary.function.c_str());
    //             // ImGui
    //         }
    //         ImGui::End();
    //     }
    //     ImGui::ShowDemoWindow();
    // }
}
}
