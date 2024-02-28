#pragma once

#include <type_traits>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImGui {

	inline void Text(const std::string& fmt, ...)
	{		
#if TOOLCHAIN_COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#endif
		// va_list args;
		// va_start(args, &fmt);
		// ImGui::Text(fmt.c_str(), args);	
		ImGui::Text(fmt.c_str(), 0);	
		// va_end(args);	
#if TOOLCHAIN_COMPILER_CLANG
#pragma clang diagnostic pop
#endif
	}

	inline void SetTooltip(const std::string& fmt, ...)
	{		
#if TOOLCHAIN_COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#endif
		// va_list args;
		// va_start(args, &fmt);
		// ImGui::Text(fmt.c_str(), args);	
		ImGui::SetTooltip(fmt.c_str(), 0);	
		// va_end(args);	
#if TOOLCHAIN_COMPILER_CLANG
#pragma clang diagnostic pop
#endif
	}

}

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void edit_integer_field(const std::string& name, value_t& value)
    {
        int _int_value = static_cast<int>(value);
        ImGui::DragInt(name.c_str(), &_int_value);
        value = static_cast<value_t>(_int_value);
    }

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value)
    {
        (void)value;
        ImGui::Text(name);
    }

}
}
