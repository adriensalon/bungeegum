#pragma once

#include <hscpp/mem/MemoryManager.h>

namespace bungeegum {
namespace detail {

    template <typename archive_t, typename... fields_t>
    void serialize_fields(archive_t& archive, const std::string& names, fields_t&&... fields)
    {
        std::vector<std::string> _names;
        std::stringstream _sstream(names);
        while (_sstream.good()) {
            std::string _substr;
            std::getline(_sstream, _substr, ',');
            if (_substr[0] == ' ') {
                _substr = _substr.substr(1, _substr.length() - 1);
            }
            _names.push_back(_substr);
        }
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

    template <typename value_t>
    reloaded<value_t>::reloaded()
    {
        static_assert(traits::is_reloadable_v<value_t>, "ERROR TODO");
    }

    template <typename value_t>
    reloaded<value_t>::reloaded(const reloaded<value_t>& other)
    {
        _ref.m_Id = other._ref.m_Id;
        _ref.m_pMemoryManager = other._ref.m_pMemoryManager;
    }

    template <typename value_t>
    reloaded<value_t>& reloaded<value_t>::operator=(const reloaded<value_t>& other)
    {
        _ref.m_Id = other._ref.m_Id;
        _ref.m_pMemoryManager = other._ref.m_pMemoryManager;
        return *this;
    }

    template <typename value_t>
    reloaded<value_t>::reloaded(reloaded<value_t>&& other)
    {
        *this = std::move(other);
    }

    template <typename value_t>
    reloaded<value_t>& reloaded<value_t>::operator=(reloaded<value_t>&& other)
    {
        _ref = std::move(other._ref);
        return *this;
    }

    template <typename value_t>
    value_t& reloaded<value_t>::get() const
    {
        return *(_ref.operator->());
    }

    template <typename value_t>
    reloaded<value_t>::reloaded(hscpp::mem::UniqueRef<value_t>&& ref)
        : _ref(std::move(ref))
    {
    }

    template <typename value_t>
    reloaded<value_t> reloader::allocate()
    {
        return _manager.get()->operator->()->Allocate<value_t>();
    }

    template <typename reloaded_value_t>
    struct value_wrapper {
        value_wrapper(reloaded_value_t& value)
            : _ref(value)
        {
        }

        template <typename archive_t>
        void load(archive_t& archive)
        {
            _ref.get()._bungeegum_load(archive);
        }

        template <typename archive_t>
        void save(archive_t& archive) const
        {
            _ref.get()._bungeegum_save(archive);
        }

        reloaded_value_t& _ref;
    };

    template <typename reloaded_value_t>
    void reloaded_loader::load(reloaded_value_t& value)
    {
        value_wrapper<reloaded_value_t> _wrapper(value);
        _archive(_wrapper);
    }

    template <typename reloaded_value_t>
    void reloaded_saver::save(reloaded_value_t& value)
    {
        value_wrapper<reloaded_value_t> _wrapper(value);
        _archive(_wrapper);
    }
}
}
