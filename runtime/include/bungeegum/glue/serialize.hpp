#pragma once

#include <string>
#include <vector>

#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

/// @brief
#define SERIALIZE_FIELDS(...)                                                               \
    friend class cereal::access;                                                            \
    template <typename archive_t>                                                           \
    void serialize(archive_t& archive)                                                      \
    {                                                                                       \
        bungeegum::detail::serialize_fields<archive_t>(archive, #__VA_ARGS__, __VA_ARGS__); \
    }

namespace bungeegum {
namespace detail {

    /// @brief
    std::vector<std::string> split_names(const std::string& comma_separated_names);

    /// @brief Serializes with cereal a struct that implements the HOTSWAP_CLASS macro with fields.
    /// @tparam archive_t is the cereal archive type. This allows us to use the same function for
    /// both load and save.
    /// @tparam ...fields_t are the fields types of this struct.
    template <typename archive_t, typename... fields_t>
    void serialize_fields(archive_t& archive, const std::string& names, fields_t&&... fields);

}
}

#include <bungeegum/glue/serialize.inl>