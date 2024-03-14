

#include <imgui.h>
#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/math.hpp>
#include <bungeegum/glue/theme.hpp>
#include <bungeegum/glue/string.hpp>

namespace bungeegum {
namespace detail {

    namespace {

        static bool filter_enabled = true;
        static std::string filter_text = "";
        
        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_output_" + name + "__";
        }

        void draw_map(const std::string& name, log_data_map& map)
        {
            std::string _title = name + " (" + std::to_string(map.size()) + ")";
            if (ImGui::BeginTabItem(_title.c_str())) {

                static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;

                // When using ScrollX or ScrollY we need to specify a size for our table container!
                // Otherwise by default the table will fit all available space, like a BeginChild() call.
                ImVec2 outer_size = ImVec2(0.0f, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().WindowPadding.y - ImGui::GetFrameHeight());

                if (ImGui::BeginTable(tag(name + "_table").c_str(), 5, flags, outer_size)) {

                    for (std::pair<const std::string, log_data>& _log : map) {
                        const std::string& _log_description = _log.second.exception.what();
                        if (!filter_enabled || (contains(_log_description, filter_text))) {
                            std::size_t& _log_count = _log.second.count;
                            const std::vector<backtraced_step>& _results = _log.second.exception.tracing;

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            bool _is_selected = false;
                            ImGui::Selectable((std::to_string(_log_count) + tag("_log_description_" + _log_description)).c_str(), &_is_selected, ImGuiSelectableFlags_SpanAllColumns);
                            if (_is_selected) {
                                _log_count = 1;
                            }

                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text(_log_description);
                            if (!_results.empty()) {
                                const backtraced_step& _last_result = _results.front();
                                ImGui::TableSetColumnIndex(2);
                                ImGui::Text(_last_result.file.filename().generic_string());
                                ImGui::TableSetColumnIndex(3);
                                ImGui::Text(("Ln " + std::to_string(_last_result.line)));
                                ImGui::TableSetColumnIndex(4);
                                ImGui::Text(("Col " + std::to_string(_last_result.column)));
                            } else {

                                ImGui::TableSetColumnIndex(2);
                                ImGui::Text("unknown file");
                                ImGui::TableSetColumnIndex(3);
                                ImGui::Text("Ln ??");
                                ImGui::TableSetColumnIndex(4);
                                ImGui::Text("Col ??");
                            }
                        }
                    }

                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }
        }
    }

    void draw_output_overlay()
    {

        ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
        if (ImGui::Begin(("logger" + tag("window")).c_str(), 0, ImGuiWindowFlags_NoCollapse)) {

            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar(tag("tab_bar").c_str(), tab_bar_flags)) {
                swapped_manager_data& _swapped = get_swapped_global();
                draw_map("errors", _swapped.errors.value().get());
                draw_map("warnings", _swapped.warnings.value().get());
                draw_map("messages", _swapped.messages.value().get());
            }
            ImGui::EndTabBar();

            ImGui::Checkbox(tag("filter_checkbox").c_str(), &filter_enabled);

            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputTextWithHint(tag("filter_input").c_str(), "type here a description to match with regex...", &filter_text)) {
                // todo
            }
            ImGui::PopItemWidth();
        }
        ImGui::End();

        // transfer_to_map(get_swapped_global().errors, error_logs);
        // transfer_to_map(get_swapped_global().warnings, warning_logs);
        // transfer_to_map(get_swapped_global().messages, message_logs);
    }
}
}