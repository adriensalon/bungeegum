#pragma once

#include <bungeegum/core/texture.fwd>

namespace bungeegum {

/// @brief
struct texture {
	
	/// @brief
	texture() = default;
	
	/// @brief 
	/// @param filename 
	texture(const std::filesystem::path& filename);
	
	/// @brief 
	/// @param pixels 
	/// @param size 
	texture(const std::vector<unsigned char>& pixels, const uint2 compiled_size);

	/// @brief
	texture(const texture& other) = default;

	/// @brief
	texture& operator=(const texture& other) = default;
	
	/// @brief
	texture(texture&& other) = default;
	
	/// @brief
	texture& operator=(texture&& other) = default;

	/// @brief  
	[[nodiscard]] uint2 get_compiled_size() const;

	/// @brief 
	/// @param pixels 
	/// @param min_point 
	/// @param max_point 
	/// @return 
	texture& region(const std::vector<unsigned char>& pixels, const uint2 min_point = zero<uint2>, const uint2 max_point = infinity<uint2>);

private:
	friend struct detail::texture_access;
	detail::texture_data _data;
};


	
	
}