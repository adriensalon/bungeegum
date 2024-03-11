#pragma once

namespace bungeegum {
namespace detail {

    template <typename swapped_t>
    std::uintptr_t& swapped_access::get_object_reference(swapped_t& object)
    {
        static_assert(traits::is_reloadable_v<swapped_t>, "ERROR TODO");
        return object._bungeegum_object_reference;
    }

    template <typename swapped_t>
    std::uintptr_t swapped_access::get_this(swapped_t& object)
    {
        static_assert(traits::is_reloadable_v<swapped_t>, "ERROR TODO");
        return object._bungeegum_this();
    }

    template <typename swapped_t>
    std::size_t swapped_access::get_sizeof(swapped_t& object)
    {
        static_assert(traits::is_reloadable_v<swapped_t>, "ERROR TODO");
        return object._bungeegum_sizeof();
    } 






    template <typename value_t>
    swapped<value_t>::swapped()
    {
        static_assert(traits::is_reloadable_v<value_t>, "ERROR TODO");
    }

    template <typename value_t>
    swapped<value_t>::swapped(const swapped<value_t>& other)
    {
        _ref.m_Id = other._ref.m_Id;
        _ref.m_pMemoryManager = other._ref.m_pMemoryManager;
    }

    template <typename value_t>
    swapped<value_t>& swapped<value_t>::operator=(const swapped<value_t>& other)
    {
        _ref.m_Id = other._ref.m_Id;
        _ref.m_pMemoryManager = other._ref.m_pMemoryManager;
        return *this;
    }

    template <typename value_t>
    swapped<value_t>::swapped(swapped<value_t>&& other)
    {
        *this = std::move(other);
    }

    template <typename value_t>
    swapped<value_t>& swapped<value_t>::operator=(swapped<value_t>&& other)
    {
        _ref = std::move(other._ref);
        return *this;
    }

    template <typename value_t>
    value_t& swapped<value_t>::get() const
    {
        return *(_ref.operator->());
    }

    template <typename value_t>
    swapped<value_t>::swapped(hscpp::mem::UniqueRef<value_t>&& ref)
        : _ref(std::move(ref))
    {
    }

    template <typename value_t>
    swapped<value_t> reloader::allocate()
    {
        return _manager.get()->operator->()->Allocate<value_t>();
    }

    template <typename value_t>
    value_t& get_global_data()
    {
        return *(hscpp::GlobalUserData::GetAs<value_t>());
    }

    template <typename value_t>
    void reloaded_loader::load(const swapped<value_t>& value)
    {
        _archive(value.get());
    }

    template <typename value_t>
    void reloaded_saver::save(const swapped<value_t>& value)
    {
        _archive(value.get());
    }
}
}
