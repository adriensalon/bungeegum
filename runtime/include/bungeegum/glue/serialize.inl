#pragma once

#include <tuple>
#include <variant>

namespace bungeegum {
namespace detail {

    template <typename archive_t, typename... fields_t>
    void serialize_fields(archive_t& archive, const std::string& names, fields_t&&... fields)
    {
        std::vector<std::string> _names = split_names(names);
        std::tuple<fields_t&...> _tuple((fields)...);
        constexpr std::size_t _count = std::variant_size_v<std::variant<fields_t...>>;
        bungeegum::detail::constexpr_for<0, _count, 1>([&](auto _index) {
            using field_type_t = std::variant_alternative_t<_index, std::variant<fields_t...>>;

            //
            // static assert field not const :)
            //

            field_type_t& _field = std::get<_index>(_tuple);
            try {
                archive(cereal::make_nvp(_names[_index].c_str(), _field));
            } catch (...) {
                std::cout << "error detected :) \n";
            }
        });
    }

}
}