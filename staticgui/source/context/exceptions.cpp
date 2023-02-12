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

#include <staticgui/context/exceptions.hpp>
#include <staticgui/context/widget.hpp>
#include <staticgui/glue/backtrace.hpp>
#include <staticgui/glue/console.hpp>

namespace staticgui {
namespace detail {
    namespace {

        static std::optional<backtraced_exception> userspace_thrown_exception = std::nullopt;
        static bool has_userspace_thrown_memory = false;

        struct library_bad_usage_exception : public backtraced_exception {
            using backtraced_exception::backtraced_exception;
        };

        struct library_bad_implementation_exception : public backtraced_exception {
            using backtraced_exception::backtraced_exception;
        };

        struct user_bad_implementation_exception : public backtraced_exception {
            using backtraced_exception::backtraced_exception;
        };
    }

    void throw_library_bad_usage(const std::string& what)
    {
        throw library_bad_usage_exception(what, backtrace_size_v);
    }

    void throw_library_bad_implementation(const std::string& what)
    {
        // if not protecting => console
        throw library_bad_implementation_exception(what, backtrace_size_v);
    }

    void protect_library(const std::function<void()>& try_callback)
    {
        // try_catch_nullptr(
        //     [&]() {
        try {
            try_callback();
        } catch (library_bad_implementation_exception& _exception) {
            std::cout << "BAD IMPL 2 \n";
            // bad implementation exception (traced)
            for (auto& _trace : _exception.tracing)
                console_log(_trace.primary.function, console_color::green);
        } catch (const std::exception& _exception) {
            std::cout << "UNKNOWN 2\n";
            std::cout << _exception.what() << std::endl;
            (void)_exception;
            // bad implementation exception (non traced)
        }
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

    std::optional<backtraced_exception>& get_userspace_thrown_exception()
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

/// @brief
/// @details
/// @param what
void throw_error(const std::string& what)
{
    throw detail::user_bad_implementation_exception(what, detail::backtrace_size_v);
}

}
