// uniform inputs only
// load from file with std or set from string

#pragma once

#include <filesystem>
#include <memory>
#include <string>

namespace bungeegum {
namespace detail {

	struct shader_fragment_options {

	};

	struct shader_blend_options {

	};

	struct shader_stencil_options {

	};

    /// @brief
    struct shader {
        shader() = delete;
        shader(const shader& other) = delete;
        shader& operator=(const shader& other) = delete;
        shader(shader&& other) = default;
        shader& operator=(shader&& other) = default;

		shader& fragment(const std::string& hlsl_source, std::initializer_list<int> args);

		shader& blend(const shader_blend_options& blend_options);

		shader& stencil(const shader_stencil_options& stencil_hlsl);

		template <typename value_t>
		shader& uniform(const std::string& uniform_name, const value_t& uniform_value);

		template <typename value_t>
		value_t get_uniform(const std::string& uniform_name);



    private:
        struct shader_data;
        std::shared_ptr<shader_data> _data = nullptr;
    };
}
}