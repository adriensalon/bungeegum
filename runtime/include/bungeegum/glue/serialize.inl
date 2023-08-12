#pragma once

#include <tuple>
#include <variant>

#include <cereal/types/array.hpp>
#include <cereal/types/atomic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/concepts/pair_associative_container.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/forward_list.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/queue.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/stack.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/valarray.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

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