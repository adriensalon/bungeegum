//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <imgui.h>
#include <implot.h>
#include <iostream>

#include <staticgui/glue/imguarded.hpp>
#include <staticgui/overlay/embedded/fa4.hpp>
#include <staticgui/overlay/overlay.hpp>
#include <staticgui/state/errors.hpp>

#include "embedded/fa4.cpp"
#include "embedded/helvetica.cpp"

namespace staticgui {
namespace detail {
    namespace overlay {

        static ImFont* overlay_font = nullptr;
        static ImFont* icons_font = nullptr;

        void install_font()
        {
            static bool _installed = false;
            if (!_installed) {

                // config.
                ImGuiIO& io = ImGui::GetIO();
                // io.Fonts->AddFontFromMemoryCompressedTTF(helvetica_compressed_data, helvetica_compressed_size, 13.0f);
                overlay_font = io.Fonts->AddFontFromMemoryCompressedTTF(helvetica_compressed_data, helvetica_compressed_size, 13.0f);

                // font awesome for the glyphs
                // ImFontConfig config;
                // config.MergeMode = true;
                // config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
                // static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
                // icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(fa4_compressed_data, fa4_compressed_size, 13.0f, &config, icon_ranges);

                // build
                io.Fonts->Build();
                _installed = true;
            }
        }

        void draw_overlay(context_state& context)
        {
            ImGui::PushFont(overlay_font);
            ImGui::ShowDemoWindow();
            if (has_userspace_thrown() || ImGui::GetIO().KeysDown[ImGuiKey_Escape]) {
                ImGui::StyleColorsLight();
                ImGui::SetNextWindowSize({ 1000, 600 });
                if (ImGui::Begin("Debug")) {
                    if (has_userspace_thrown()) {
                        glue::backtraced_exception& _exception = get_userspace_thrown_exception().value();
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
                        context.widgets.iterate_datas([&](detail::widget_data& _widget_data) {
                            unsigned int _depth = context.widgets.get_depth(_widget_data);
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
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();
                    }

                    int _k = 0;
                    context.animations.iterate_datas([&](detail::animation_data& _animation_data) {
                        if (_animation_data.is_playing) {
                            std::string _title = "##StatsGraphTitle" + std::to_string(_k);
                            if (ImPlot::BeginPlot(_title.c_str(), { ImGui::GetContentRegionAvail().x, 150 })) {
                                auto _samples = _animation_data.curve.spline->get_strided_samples(100);
                                auto _point = _animation_data.t_curve;
                                ImPlot::PlotLine("", _samples.data(), &(_samples[1]), 100, 0, 0, 2 * sizeof(float));
                                ImPlot::PlotScatter("", _point.data(), _point.data() + 1, 1, 0, 0, 2 * sizeof(float));
                                ImPlot::EndPlot();
                            }
                        }
                        _k++;
                    });
                    ImGui::End();
                }
            }
            ImGui::PopFont();
        }
    }
}
}