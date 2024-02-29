#pragma once

#include <bungeegum/core/shader.fwd>

namespace bungeegum {

// BLEND

/// @brief
struct shader_resource {

    /// @brief
    /// @param source
    /// @param args
    /// @return
    shader_resource& fragment(const std::string& source, std::initializer_list<int> args);

    /// @brief
    /// @param options
    /// @return
    // shader_resource& blend(const shader_depth_options& options);

    // uniforms

    // textures

private:
    friend struct detail::shader_resource_access;
    shader_resource(const detail::shader_resource_data& data);
    detail::shader_resource_data _data;
};

/// @brief
/// @details Deep copy + move
struct shader {

    /// @brief 
    /// @param resource 
    /// @return 
    shader& emplace(const shader_resource& resource);
    
    /// @brief Gets if this shader has a value. Default created shaders don't have a value until 
    /// the emplace() method is called. Shaders don't have a value anymore after the reset() method
    /// is called
    [[nodiscard]] bool has_value() const;

    /// @brief Resets this shader if it has a value. Default created shaders don't have a value 
    /// until the emplace() method is called. Shaders don't have a value anymore after the reset()
    /// method is called
    shader& reset();

    /// @brief
    /// @tparam value_t
    /// @param name
    /// @param value
    /// @return
    template <typename value_t>
    shader& uniform(const std::string& name, const value_t& value);

private:
    friend struct detail::shader_access;
    shader(const detail::shader_data& data);
    detail::shader_data _data;
};

}