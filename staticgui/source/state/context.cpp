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
#include <imgui_internal.h>
#include <implot.h>
#include <iostream>

#include <staticgui/glue/backtrace.hpp>
#include <staticgui/glue/bspline.hpp>
#include <staticgui/state/context.hpp>
#include <staticgui/state/errors.hpp>

namespace staticgui {
namespace detail {

    void draw_bezier()
    {
        // ImPlot::ShowDemoWindow();
        if (ImGui::Begin("Bezier")) {

            std::vector<float> controls = { 100.f, -100.f,
                -100.f, 200.f,
                100.f, 400.f,
                400.f, 300.f,
                700.f, 500.f };
            std::vector<glue::simd_array<float, 2>> controls2 = std::vector<glue::simd_array<float, 2>> {
                std::array<float, 2> { 100.f, -100.f },
                std::array<float, 2> { -100.f, 200.f },
                std::array<float, 2> { 100.f, 400.f },
                std::array<float, 2> { 400.f, 300.f },
                std::array<float, 2> { 700.f, 500.f }
            };

            auto controls3 = std::vector<glue::simd_array<float, 2>> {
                std::array<float, 2> { 0.25f, 0.15f },
                std::array<float, 2> { 0.75f, 0.85f }
            };

            auto _spl = glue::bspline(0.f, 1.f, controls3); //controls2);
            std::vector<float> kkk = _spl.get_strided_samples(100);
            std::cout << "eval t = 0.6, y = " << _spl.get_eval(0.6f) << std::endl;

            if (ImPlot::BeginPlot("##StatsGraphTitle", ImGui::GetContentRegionAvail())) {
                //     if (state.train_flow_index() > 0) {
                //         std::vector<float> x(state.train_flow_index());
                //         for (int k = 0; k < state.train_flow_index(); k++)
                //             x[k] = k + 1;
                //         auto xptr = x.data();
                // ImPlot::PlotScatter("pp", controls.data(), &(controls[1]), 5, 0, 0, 2 * sizeof(float));
                ImPlot::PlotLine("gan loss", kkk.data(), &(kkk[1]), 100, 0, 0, 2 * sizeof(float));
                //         ImPlot::PlotLine("L1 loss", xptr, state.train_stats().get_l1_loss_data(), state.train_flow_index());
                //         ImPlot::PlotLine("real loss", xptr, state.train_stats().get_real_loss_data(), state.train_flow_index());
                //         ImPlot::PlotLine("fake loss", xptr, state.train_stats().get_fake_loss_data(), state.train_flow_index());

                ImPlot::EndPlot();
            }
            ImGui::End();
        }
    }
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //

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
            draw_bezier();
        }
    }
}
}