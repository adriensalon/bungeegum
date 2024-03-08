#pragma once

#include <bungeegum/core/font.fwd>

namespace bungeegum {

/// @brief
struct font {
	
	/// @brief
	font() = default;
	
	/// @brief
	font(const void* ttf, const std::size_t count, const float size);
	
	/// @brief
	font(const font& other) = default;
	
	/// @brief
	font& operator=(const font& other) = default;
	
	/// @brief
	font(font&& other) = default;
	
	/// @brief
	font& operator=(font&& other) = default;

	/// @brief
	font& compile(const void* ttf, const std::size_t count, const float size);

	/// @brief
    [[nodiscard]] bool is_compiled() const;

private:
	friend struct detail::font_access;
	detail::font_data _data;
};

}