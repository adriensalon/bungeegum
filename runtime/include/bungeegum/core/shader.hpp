#pragma once

#include <bungeegum/core/shader.fwd>

namespace bungeegum {

struct shader_fragment_options {
};

struct shader_blend_options {
};

struct shader_stencil_options {
};

struct shader_resource {
    shader_resource() = default;
    shader_resource(const shader_resource& other) = default;
    shader_resource& operator=(const shader_resource& other) = default;
    shader_resource(shader_resource&& other) = default;
    shader_resource& operator=(shader_resource&& other) = default;

    /// @brief 
    /// @param source 
    /// @param args 
    /// @return 
    shader_resource& fragment(const std::string& source, std::initializer_list<int> args);

    /// @brief 
    /// @param options 
    /// @return 
    shader_resource& blend(const shader_blend_options& options);

    /// @brief 
    /// @param options 
    /// @return 
    shader_resource& stencil(const shader_stencil_options& options);
	
private:
	detail::shader_resource_data _data;
};

/// @brief
struct shader_ref {
    shader_ref() = delete;
    shader_ref(const shader_ref& other) = default;
    shader_ref& operator=(const shader_ref& other) = default;
    shader_ref(shader_ref&& other) = default;
    shader_ref& operator=(shader_ref&& other) = default;

    /// @brief 
    /// @tparam value_t 
    /// @param name 
    /// @param value 
    /// @return 
    template <typename value_t>
    shader_ref& uniform(const std::string& name, const value_t& value);

private:
	friend struct detail::shader_ref_access;
    shader_ref(const detail::shader_ref_data& data);
	detail::shader_ref_data _data;
};

/// @brief 
/// @param name 
/// @param resource 
/// @return 
[[nodiscard]] shader_ref get_shader(const std::string& name);

/// @brief 
/// @param name 
/// @param resource 
/// @return 
shader_ref make_shader(const std::string& name, shader_resource& resource);

}