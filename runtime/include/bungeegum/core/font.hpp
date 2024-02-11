#pragma once

#include <bungeegum/core/font.fwd>

namespace bungeegum {

struct font_resource {
	font_resource() = default;
	font_resource(const font_resource& other) = default;
	font_resource& operator=(const font_resource& other) = default;
	font_resource(font_resource&& other) = default;
	font_resource& operator=(font_resource&& other) = default;

	/// @brief 
	/// @param ttf 
	/// @param count 
	/// @return 
	font_resource& compressed(const void* ttf, const std::size_t count);

	/// @brief 
	/// @param size 
	/// @return 
	font_resource& size(const float size);

private:
	friend struct detail::font_resource_access;
    font_resource(const detail::font_resource_data& data);
	detail::font_resource_data _data;
};

/// @brief
struct font_ref {
	font_ref() = delete;
	font_ref(const font_ref& other) = default;
	font_ref& operator=(const font_ref& other) = default;
	font_ref(font_ref&& other) = default;
	font_ref& operator=(font_ref&& other) = default;

private:
	friend struct detail::font_ref_access;
    font_ref(const detail::font_ref_data& data);
	detail::font_ref_data _data;
};

/// @brief 
/// @param name 
/// @param resource 
/// @return 
[[nodiscard]] font_ref get_font(const std::string& name);

/// @brief 
/// @param name 
/// @param resource 
/// @return 
font_ref make_font(const std::string& name, font_resource& resource);

}