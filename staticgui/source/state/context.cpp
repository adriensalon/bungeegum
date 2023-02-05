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
#include <iostream>

#include <staticgui/glue/backtrace.hpp>
#include <staticgui/state/context.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    bool context_state::tick(const float delta_milliseconds)
    {
        animations.tick(delta_milliseconds);
        events.tick();

        // on window resize on resolve
        bool _has_window_been_resized = false;

        // resolve si besoin
        // bool _must_draw = (!widgets.is_must_resolve_empty() && !widgets.is_must_draw_empty());
        bool _must_draw = (!widgets.is_must_draw_empty());
        widgets.iterate_must_resolve([&](widget_data& _data, const bool _must_resolve_children) {

        });
        widgets.clear_resolve();

        return _must_draw;
    }

    void context_state::draw()
    {
        // throw_library_bad_usage("whatt");
        //
        //
        //
        //
        widgets.iterate_must_draw([](widget_data& _data, const bool _must_draw_children) {
            if (!has_userspace_thrown())
                _data.command.value().clear();
            protect_userspace([&]() {
                _data.drawer(_data.command.value());
            });
        });
        widgets.iterate_datas([](widget_data& _data) {
            if (_data.command.has_value())
                _data.command.value().draw();
        });
        if (!has_userspace_thrown())
            widgets.clear_draw();

        // draw exception if any
        if (has_userspace_thrown()) {
            glue::backtraced_exception& _exception = get_userspace_thrown_exception().value();
            ImGui::StyleColorsLight();
            if (ImGui::Begin("Exception caught")) {
                ImGui::Text(_exception.what());
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                for (auto& _trace : _exception.tracing) {
                    ImGui::TextColored(ImVec4(0.1f, 0.1f, 0.8f, 1.f), _trace.primary.function.c_str());
                }
                auto _black = ImVec4(0.f, 0.f, 0.f, 1.f);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                widgets.iterate_datas([&](detail::widget_data& _widget_data) {
                    unsigned int _depth = widgets.get_depth(_widget_data);
                    for (unsigned int _k = 0; _k < _depth; _k++) {
                        ImGui::TextColored(_black, "      ");
                        ImGui::SameLine();
                    }
                    ImGui::TextColored(_black, _widget_data.kind->name());
                    if (_widget_data.drawer) {
                        ImGui::SameLine();
                        ImGui::TextColored(_black, " [painter]");
                    }
                });
                ImGui::End();
            }
        }
    }
}
}