#pragma once

#include <cstddef>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <bungeegum/glue/backtrace.hpp>

namespace bungeegum {
namespace detail {

    /// @brief Instances of this struct store key-value pairs with a design that favors fast
    /// iteration, while only adding an indexing operation for accessing a value from a key.
    /// @details Instances of this struct can be copied and moved.
    /// @tparam key_t is the type to use for keys. This type must meet the requirements of a
    /// in a std::unordered_map key type.
    /// @tparam value_t is the type to use for values. This type must meet the requirements of a
    /// std::vector value type.
    template <typename key_t, typename value_t>
    struct indexed_map {
        indexed_map() = default;
        indexed_map(const indexed_map& other) = default;
        indexed_map& operator=(const indexed_map& other) = default;
        indexed_map(indexed_map&& other) = default;
        indexed_map& operator=(indexed_map&& other) = default;

        /// @brief Iterator type to use with std algorithms and range loops
        using iterator = std::vector<value_t>::template iterator;

        /// @brief Const iterator type to use with std algorithms and range loops
        using const_iterator = std::vector<value_t>::template const_iterator;

        /// @brief Creates an instance from a set of key-value pairs.
        indexed_map(std::initializer_list<std::pair<key_t, value_t>> values);

        /// @brief Gets a reference to a value from its key.
        /// @exception Throws a backtraced exception if the key is not contained.
        [[nodiscard]] value_t& at(const key_t& key);

        /// @brief Gets a const reference to a value from its key.
        /// @exception Throws a backtraced exception if the key is not contained.
        [[nodiscard]] const value_t& at(const key_t& key) const;

        /// @brief Gets a begin iterator to use with range loops.
        [[nodiscard]] iterator begin();

        /// @brief Gets a const begin iterator to use with range loops.
        [[nodiscard]] const_iterator begin() const;

        /// @brief Gets if a key is contained.
        [[nodiscard]] bool contains(const key_t& key);

        /// @brief Emplaces a key-value pair.
        /// @exception Throws a backtraced exception if the key is already contained.
        value_t& emplace(const key_t& key, const value_t& value);

        /// @brief Erases a key-value pair from a key.
        /// @exception Throws a backtraced exception if the key is not contained.
        void erase(const key_t& key);

        /// @brief Gets a end iterator to use with range loops.
        [[nodiscard]] iterator end();

        /// @brief Gets a const end iterator to use with range loops.
        [[nodiscard]] const_iterator end() const;

        /// @brief Gets a reference to a value from its key.
        /// @details A default-constructed value will be created if the key is not contained.
        /// @exception Throws a compile-time exception if the type is not default-constructible.
        [[nodiscard]] value_t& operator[](const key_t& key);

        /// @brief Gets a const reference to a value from its key.
        /// @details A default-constructed value will be created if the key is not contained.
        /// @exception Throws a compile-time exception if the type is not default-constructible.
        [[nodiscard]] const value_t& operator[](const key_t& key) const;

        /// @brief Length of the trace when collecting runtime backtraced exceptions.
        inline static std::size_t backtrace_size = 10u;

    private:
        std::vector<value_t> _values = {};
        std::unordered_map<key_t, std::size_t> _indices = {};
    };
}
}

#include <bungeegum/glue/map.inl>