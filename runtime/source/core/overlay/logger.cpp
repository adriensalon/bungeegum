

#include <imgui.h>
#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/glue/imguarded.fwd>
#include <bungeegum/glue/regex.hpp>
#include <bungeegum/glue/simd.hpp>


#define BUNGEEGUM_USE_OVERLAY_LOGGER_MAX_MESSAGE_LENGTH 300u
#define BUNGEEGUM_USE_OVERLAY_LOGGER_MAX_MESSAGES 999u

namespace bungeegum {
namespace detail {

    namespace {
        using backtraced_results = std::vector<backtraced_result>;
        using counted_backtraced_results = std::pair<std::size_t, backtraced_results>;

        static std::unordered_map<std::string, counted_backtraced_results> error_logs = {};
        static std::unordered_map<std::string, counted_backtraced_results> warning_logs = {};
        static std::unordered_map<std::string, counted_backtraced_results> message_logs = {};

        static std::string filter_text = "";
        static bool filter_enabled = true;

        std::string tag(const std::string& name)
        {
            return "###__bungeegum_overlay_logger_" + name + "__";
        }

        std::string truncate_to_key(
            const std::string str,
            const uint1 max_length = BUNGEEGUM_USE_OVERLAY_LOGGER_MAX_MESSAGE_LENGTH)
        {
            return str.substr(0, math::max(static_cast<uint1>(str.length()), max_length));
        }

        void transfer_to_map(
            std::vector<backtraced_exception>& from,
            std::unordered_map<std::string, counted_backtraced_results>& to,
            const uint1 max_count = BUNGEEGUM_USE_OVERLAY_LOGGER_MAX_MESSAGES)
        {
            for (backtraced_exception& _exception : from) {
                std::string _key = truncate_to_key(_exception.what());
                if (to.find(_key) == to.end()) {
                    to.emplace(_key, std::make_pair<std::size_t, backtraced_results>(1u, std::move(_exception.tracing)));
                } else if (to.at(_key).first < max_count) {
                    to.at(_key).first++;
                }
            }
            from.clear();
        }

        void draw_map(const std::string& name, const std::size_t count, std::unordered_map<std::string, counted_backtraced_results>& map)
        {
            std::string _title = name + " (" + std::to_string(count) + ")" + tag(name + "_tab");
            if (ImGui::BeginTabItem(_title.c_str())) {

                static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;

                // When using ScrollX or ScrollY we need to specify a size for our table container!
                // Otherwise by default the table will fit all available space, like a BeginChild() call.
                ImVec2 outer_size = ImVec2(0.0f, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().WindowPadding.y - ImGui::GetFrameHeight());

                if (ImGui::BeginTable(tag(name + "_table").c_str(), 5, flags, outer_size)) {

                    for (std::pair<const std::string, counted_backtraced_results>& _log : map) {
                        const std::string& _log_description = _log.first;
                        if (!filter_enabled || (regex_search(_log_description, filter_text))) {
                            std::size_t& _log_count = _log.second.first;
                            const std::vector<backtraced_result>& _results = _log.second.second;

                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            bool _is_selected = false;
                            ImGui::Selectable((std::to_string(_log_count) + tag("_log_description_" + _log_description)).c_str(), &_is_selected, ImGuiSelectableFlags_SpanAllColumns);
                            if (_is_selected) {
                                _log_count = 1;
                            }

                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text(_log_description.c_str());
                            if (!_results.empty()) {
                                const backtraced_result& _last_result = _results.front();
                                ImGui::TableSetColumnIndex(2);
                                ImGui::Text(_last_result.primary.file.filename().generic_string().c_str());
                                ImGui::TableSetColumnIndex(3);
                                ImGui::Text(("Ln " + std::to_string(_last_result.primary.line)).c_str());
                                ImGui::TableSetColumnIndex(4);
                                ImGui::Text(("Col " + std::to_string(_last_result.primary.column)).c_str());
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

    void draw_logger_overlay()
    {

        ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
        if (ImGui::Begin(("logger" + tag("window")).c_str(), 0, ImGuiWindowFlags_NoCollapse)) {

            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar(tag("tab_bar").c_str(), tab_bar_flags)) {
                draw_map("errors", error_logs.size(), error_logs);
                draw_map("warnings", warning_logs.size(), warning_logs);
                draw_map("messages", message_logs.size(), message_logs);
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

        transfer_to_map(global().logs.userspace_errors, error_logs);
        transfer_to_map(global().logs.userspace_warnings, warning_logs);
        transfer_to_map(global().logs.userspace_messages, message_logs);
    }
}
}