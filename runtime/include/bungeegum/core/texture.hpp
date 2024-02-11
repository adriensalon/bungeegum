#pragma once

#include <bungeegum/core/texture.fwd>

namespace bungeegum {

/// @brief
struct texture_resource {
	texture_resource() = default;
	texture_resource(const texture_resource& other) = default;
	texture_resource& operator=(const texture_resource& other) = default;
	texture_resource(texture_resource&& other) = default;
	texture_resource& operator=(texture_resource&& other) = default;

	/// @brief 
	/// @param float_pixels 
	/// @return 
	texture_resource& pixels(const std::vector<unsigned char>& char_pixels);

	/// @brief 
	/// @param height 
	/// @param width 
	/// @return 
	texture_resource& size(const std::size_t height, const std::size_t width);
	
private:
	friend struct detail::texture_resource_access;
    texture_resource(const detail::texture_resource_data& data);
	detail::texture_resource_data _data;
};

/// @brief
struct texture_ref {
	texture_ref() = default;
	texture_ref(const texture_ref& other);
	texture_ref& operator=(const texture_ref& other);
	texture_ref(texture_ref&& other);
	texture_ref& operator=(texture_ref&& other);

	// modify pixels ?

private:
	friend struct detail::texture_ref_access;
    texture_ref(const detail::texture_ref_data& data);
	detail::texture_ref_data _data;
};

/// @brief 
/// @param name 
/// @param resource 
/// @return 
[[nodiscard]] texture_ref get_texture(const std::string& name);


/// @brief 
/// @param name 
/// @param resource 
/// @return 
texture_ref make_texture(const std::string& name, texture_resource& resource);
	
	
}