#pragma once

namespace bungeegum {
namespace detail {

    // unique_reference

    template <typename widget_t>
    unique_reference<widget_t>::unique_reference(unique_reference<widget_t>&& other)
    {
        _ref.MoveRef(std::move(other._ref));
    }

    template <typename widget_t>
    unique_reference<widget_t>& unique_reference<widget_t>::operator=(unique_reference<widget_t>&& other)
    {
        _ref.Free();
        _ref.MoveRef(std::move(other._ref));
        return *this;
    }

    template <typename widget_t>
    widget_t& unique_reference<widget_t>::get()
    {
        return *(_ref.GetMemory());
    }

    template <typename widget_t>
    const widget_t& unique_reference<widget_t>::get() const
    {
        return *(_ref.GetMemory());
    }

    template <typename widget_t>
    unique_reference<widget_t>::unique_reference(hscpp::mem::UniqueRef<widget_t>&& ref)
        : _ref(std::move(ref))
    {
    }

    // free

    template <typename widget_t>
    unique_reference<widget_t> allocate()
    {
    }
}
}

#include <bungeegum/glue/reload.inl>