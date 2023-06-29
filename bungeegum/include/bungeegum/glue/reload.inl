#pragma once

namespace bungeegum {
namespace detail {

    // unique_reference

    template <typename widget_t>
    unique_reference<widget_t>::unique_reference(unique_reference<widget_t>&& other)
    {
        *this = std::move(other);
    }

    template <typename widget_t>
    unique_reference<widget_t>& unique_reference<widget_t>::operator=(unique_reference<widget_t>&& other)
    {
        _ref = std::move(other._ref);
        return *this;
    }

    template <typename widget_t>
    widget_t& unique_reference<widget_t>::get()
    {
        return *(_ref.operator->());
    }

    template <typename widget_t>
    const widget_t& unique_reference<widget_t>::get() const
    {
        return *(_ref.operator->());
    }

    template <typename widget_t>
    unique_reference<widget_t>::unique_reference(hscpp::mem::UniqueRef<widget_t>&& ref)
        : _ref(std::move(ref))
    {
    }

    // reload_manager

    template <typename widget_t>
    unique_reference<widget_t> reload_manager::allocate()
    {
        return _manager.get().template Allocate<widget_t>();
    }
}
}

#include <bungeegum/glue/reload.inl>