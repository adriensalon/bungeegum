#pragma once

#include <bungeegum/core/shader.fwd>

namespace bungeegum {

/// @brief
enum struct shader_depth_comparison_function {

    /// @brief
    never,

    /// @brief
    less,

    /// @brief
    equal,

    /// @brief
    less_equal,

    /// @brief
    greater,

    /// @brief
    not_equal,

    /// @brief
    greater_equal,

    /// @brief
    always
};

/// @brief
struct shader_depth_options {
    shader_depth_options() = default;
    shader_depth_options(const shader_depth_options& other) = default;
    shader_depth_options& operator=(const shader_depth_options& other) = default;
    shader_depth_options(shader_depth_options&& other) = default;
    shader_depth_options& operator=(shader_depth_options&& other) = default;

    /// @brief
    /// @param value
    void enable(const bool value);

    /// @brief
    /// @param value
    void enable_write(const bool value);

    /// @brief
    /// @param function
    void function(const shader_depth_comparison_function function);

private:
    friend struct detail::shader_depth_options_access;
    shader_depth_options(const detail::shader_depth_options_data& data);
    detail::shader_depth_options_data _data;
};

/// @brief
enum struct shader_stencil_op {

    /// @brief
    keep,

    /// @brief
    zero,

    /// @brief
    replace,

    /// @brief
    incr_sat,

    /// @brief
    decr_sat,

    /// @brief
    invert,

    /// @brief
    incr_wrap,

    /// @brief
    decr_wrap
};

/// @brief
struct shader_stencil_options {
    shader_stencil_options() = default;
    shader_stencil_options(const shader_stencil_options& other) = default;
    shader_stencil_options& operator=(const shader_stencil_options& other) = default;
    shader_stencil_options(shader_stencil_options&& other) = default;
    shader_stencil_options& operator=(shader_stencil_options&& other) = default;

    /// @brief
    void enable(const bool value);

    /// @brief
    void read_mask(const std::uint8_t mask);

    /// @brief
    void write_mask(const std::uint8_t mask);

    /// @brief
    void face_op(const shader_stencil_op op);

private:
    friend struct detail::shader_stencil_options_access;
    shader_stencil_options(const detail::shader_stencil_options_data& data);
    detail::shader_stencil_options_data _data;
};

/// @brief
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
    shader_resource& depth(const shader_depth_options& options);

    /// @brief
    /// @param options
    /// @return
    shader_resource& stencil(const shader_stencil_options& options);

private:
    friend struct detail::shader_resource_access;
    shader_resource(const detail::shader_resource_data& data);
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