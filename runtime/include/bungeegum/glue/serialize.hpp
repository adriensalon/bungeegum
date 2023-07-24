#pragma once

#include <string>
#include <vector>

#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

#define SERIALIZE_FIELDS(...)                                                               \
    friend class cereal::access;                                                            \
    template <typename archive_t>                                                           \
    void serialize(archive_t& archive)                                                      \
    {                                                                                       \
        bungeegum::detail::serialize_fields<archive_t>(archive, #__VA_ARGS__, __VA_ARGS__); \
    }

namespace bungeegum {
namespace detail {

    std::vector<std::string> split_names(const std::string& comma_separated_names);

    template <typename archive_t, typename... fields_t>
    void serialize_fields(archive_t& archive, const std::string& names, fields_t&&... fields);

}
}

#include <bungeegum/glue/serialize.inl>