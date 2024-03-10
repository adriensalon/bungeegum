#pragma once

#include <bungeegum/core/shader.fwd>

namespace bungeegum {

/// @brief
enum struct shader_blend_factor {
    
    /// The blend factor is zero.\n
    zero,
    
    /// The blend factor is one.\n
    one,
    
    /// The blend factor is RGB data from a pixel shader.\n
    src_color,
    
    /// The blend factor is 1-RGB, where RGB is the data from a pixel shader.\n
    inv_src_color,
    
    /// The blend factor is alpha (A) data from a render target.\n
    src_alpha,
    
    /// The blend factor is 1-A, where A is alpha data from a pixel shader.\n
    inv_src_alpha,

    /// The blend factor is RGB data from a render target.\n
    dest_color,

    /// The blend factor is 1-RGB, where RGB is the data from a render target.\n
    inv_dest_color,
    
    /// The blend factor is alpha (A) data from a render target.\n
    dest_alpha,

    /// The blend factor is 1-A, where A is alpha data from a render target.\n
    inv_dest_alpha,
};

/// @brief
enum struct shader_blend_operation {

    /// Add source and destination color components.\n
    add,

    /// Subtract destination color components from source color components.\n
    subtract,

    /// Subtract source color components from destination color components.\n
    rev_subtract,

    /// Compute the minimum of source and destination color components.\n
    min,

    /// Compute the maximum of source and destination color components.\n
    max

};

/// @brief
struct shader_blend_options {

    /// @brief
    bool enable = false;

    /// @brief
    shader_blend_factor src = shader_blend_factor::src_alpha;

    /// @brief
    shader_blend_factor dest = shader_blend_factor::inv_src_alpha;

    /// @brief
    shader_blend_operation op = shader_blend_operation::add;

    /// @brief
    shader_blend_factor src_alpha = shader_blend_factor::inv_src_alpha;

    /// @brief
    shader_blend_factor dest_alpha = shader_blend_factor::zero;

    /// @brief
    shader_blend_operation alpha_op = shader_blend_operation::add;
};

/// @brief
struct shader {

    /// @brief
    shader() = default;

    /// @brief 
    /// @param fragment 
    /// @param blend 
    shader(const std::string& fragment, const shader_blend_options& blend = {});

    /// @brief
    /// @param other
    shader(const shader& other) = default;

    /// @brief
    /// @param other
    shader& operator=(const shader& other) = default;

    /// @brief
    /// @param other
    shader(shader&& other) = default;

    /// @brief
    /// @param other
    shader& operator=(shader&& other) = default;

    /// @brief 
    /// @param fragment 
    /// @param blend 
    shader& compile(const std::string& fragment, const shader_blend_options& blend = {});
    
    /// @brief Gets if this shader has a value. Default created shaders don't have a value until 
    /// the emplace() method is called. Shaders don't have a value anymore after the reset() method
    /// is called
    [[nodiscard]] bool is_compiled() const;

    /// @brief
    /// @tparam value_t
    /// @param name
    /// @param value
    /// @return
    template <typename value_t>
    shader& uniform(const std::string& name, const value_t& value);

private:
    friend struct detail::shader_access;
    detail::shader_data _data;
};

}