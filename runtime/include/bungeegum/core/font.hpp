#pragma once

#include <bungeegum/core/font.fwd>

namespace bungeegum {

/// @brief
struct font {
	
	/// @brief
	font() = default;
	
	/// @brief 
	/// @param filename 
	/// @param compiled_size 
	font(const std::filesystem::path& filename, const float compiled_size);
	
	/// @brief 
	/// @param ttf 
	/// @param count 
	/// @param compiled_size 
	font(const void* ttf, const std::size_t count, const float compiled_size);
	
	/// @brief
	font(const font& other) = default;
	
	/// @brief
	font& operator=(const font& other) = default;
	
	/// @brief
	font(font&& other) = default;
	
	/// @brief
	font& operator=(font&& other) = default;

	/// @brief  
	[[nodiscard]] float get_compiled_size() const;

private:
	friend struct detail::font_access;
	detail::font_data _data;
};

}