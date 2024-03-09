#pragma once

#include <bungeegum/core/texture.fwd>

namespace bungeegum {

/// @brief
struct texture {
	
	/// @brief
	texture() = default;
	
	/// @brief
	texture(const std::vector<unsigned char>& pixels, const uint2 size);
	
	/// @brief
	texture(const void* handle);

	/// @brief
	texture(const texture& other) = default;

	/// @brief
	texture& operator=(const texture& other) = default;
	
	/// @brief
	texture(texture&& other) = default;
	
	/// @brief
	texture& operator=(texture&& other) = default;
	
	/// @brief
	texture& compile(const std::filesystem::path& filename);
	
	/// @brief
	texture& compile(const std::vector<unsigned char>& pixels, const uint2 size);

	/// @brief
	texture& import(const void* handle);
		
	/// @brief
    [[nodiscard]] bool is_compiled() const;

	/// @brief
	texture& region(const std::vector<unsigned char>& pixels, const uint2 min_point = zero<uint2>, const uint2 max_point = infinity<uint2>);

private:
	friend struct detail::texture_access;
	detail::texture_data _data;
};
	
	
}