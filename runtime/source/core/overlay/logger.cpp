

#include <imgui.h>
#include <implot.h>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/overlay.fwd>
#include <bungeegum/glue/simd.hpp>

#define BUNGEEGUM_USE_OVERLAY_LOGGER_MAX_MESSAGE_LENGTH 300u
#define BUNGEEGUM_USE_OVERLAY_LOGGER_MAX_MESSAGES 999u

namespace bungeegum {
namespace detail {

    namespace {
        using backtraced_results = std::vector<backtraced_result>;
        using counted_backtraced_results = std::pair<std::size_t, backtraced_results>;

        std::unordered_map<std::string, counted_backtraced_results> error_logs = {};
        std::unordered_map<std::string, counted_backtraced_results> warning_logs = {};
        std::unordered_map<std::string, counted_backtraced_results> message_logs = {};

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
    }

    void draw_logger_overlay()
    {
        transfer_to_map(global().logs.userspace_errors, error_logs);
        transfer_to_map(global().logs.userspace_warnings, warning_logs);
        transfer_to_map(global().logs.userspace_messages, message_logs);

        ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
        if (ImGui::Begin("logger##__bungeegum_window_logger_title__", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_Modal)) {

            static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBodyUntilResize;

            // When using ScrollX or ScrollY we need to specify a size for our table container!
            // Otherwise by default the table will fit all available space, like a BeginChild() call.
            ImVec2 outer_size = ImVec2(0.0f, ImGui::GetContentRegionAvail().y);
            if (ImGui::BeginTable("table_scrolly", 4, flags, outer_size)) {
                // ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                // ImGui::TableSetupColumn("One");
                // ImGui::TableSetupColumn("Two");
                // ImGui::TableSetupColumn("Three");
                // ImGui::TableHeadersRow();

                // Demonstrate using clipper for large vertical lists
                ImGuiListClipper clipper;
                clipper.Begin(1000);
                while (clipper.Step()) {
                    for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                        ImGui::TableNextRow();
                        // for (int column = 0; column < 3; column++) {
                        ImGui::TableSetColumnIndex(0);
                        // static bool _iss = false;
                        ImGui::Selectable("overlay.cpp", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("Ln 254");
                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("Col 0");
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("bungeegum::detail::draw_overlay");
                        // }
                    }
                }
                ImGui::EndTable();
            }

            for (auto& _log : error_logs) {
                ImGui::Text(std::to_string(_log.second.first).c_str());
                ImGui::SameLine();
                ImGui::Text(_log.first.c_str());
            }

            for (auto& _log : warning_logs) {
                ImGui::Text(std::to_string(_log.second.first).c_str());
                ImGui::SameLine();
                ImGui::Text(_log.first.c_str());
            }

            for (auto& _log : message_logs) {
                ImGui::Text(std::to_string(_log.second.first).c_str());
                ImGui::SameLine();
                ImGui::Text(_log.first.c_str());
            }
        }
        ImGui::End();
    }

}
}