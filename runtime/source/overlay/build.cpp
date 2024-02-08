

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/log.hpp>
#include <bungeegum/core/math.hpp>
#include <bungeegum/glue/imutil.hpp>
#include <bungeegum/glue/regex.hpp>

namespace bungeegum {
namespace detail {

    namespace {
		
	}

    void draw_build_overlay()
    {

        ImGui::SetNextWindowSize({ 800, 250 }, ImGuiCond_Once);
        if (ImGui::Begin(std::string("build").c_str(), 0, ImGuiWindowFlags_NoCollapse)) {

        }
        ImGui::End();
    }
}
}