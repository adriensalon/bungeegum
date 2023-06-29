#pragma once
#include <hscpp/mem/MemoryManager.h>

namespace bungeegum {
namespace detail {

    // reloaded

    template <typename widget_t>
    reloaded<widget_t>::reloaded(reloaded<widget_t>&& other)
    {
        *this = std::move(other);
    }

    template <typename widget_t>
    reloaded<widget_t>& reloaded<widget_t>::operator=(reloaded<widget_t>&& other)
    {
        _ref = std::move(other._ref);
        return *this;
    }

    template <typename widget_t>
    widget_t& reloaded<widget_t>::get()
    {
        return *(_ref.operator->());
    }

    template <typename widget_t>
    const widget_t& reloaded<widget_t>::get() const
    {
        return *(_ref.operator->());
    }

    template <typename widget_t>
    reloaded<widget_t>::reloaded(hscpp::mem::UniqueRef<widget_t>&& ref)
        : _ref(std::move(ref))
    {
    }

    // reload_manager

    template <typename widget_t>
    reloaded<widget_t> reloader::allocate()
    {
        return _manager.get()->operator->()->Allocate<widget_t>();
    }
}
}

#include <bungeegum/glue/reload.inl>