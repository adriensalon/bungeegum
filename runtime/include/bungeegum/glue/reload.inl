#pragma once

#include <hscpp/mem/MemoryManager.h>

namespace bungeegum {
namespace detail {

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

    template <typename value_t>
    value_t& get_global_data()
    {
        return *(hscpp::GlobalUserData::GetAs<value_t>());
    }

    template <typename value_t>
    struct reloaded_wrapper {
        reloaded_wrapper(reloaded<value_t>& value)
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

        reloaded<value_t>& _ref;
    };

    template <typename value_t>
    void reloaded_loader::load(reloaded<value_t>& value)
    {
        reloaded_wrapper<value_t> _wrapper(value);
        _archive(_wrapper);
    }

    template <typename value_t>
    void reloaded_saver::save(reloaded<value_t>& value)
    {
        reloaded_wrapper<value_t> _wrapper(value);
        _archive(_wrapper);
    }
}
}
