#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    namespace detail {

        template <typename delegate_t>
        using getConstraintsForChildFunction = decltype(std::declval<delegate_t>().getConstraintsForChild(std::declval<const BoxConstraints>()));

        template <typename delegate_t>
        using getPositionForChildFunction = decltype(std::declval<delegate_t>().getPositionForChild(std::declval<const Size>(), std::declval<const Size>()));

        template <typename delegate_t>
        using getSizeFunction = decltype(std::declval<delegate_t>().getSize(std::declval<const BoxConstraints>()));

        template <typename delegate_t>
        constexpr inline bool hasGetConstraintsForChildFunction = bungeegum::detail::is_detected_exact_v<BoxConstraints, getConstraintsForChildFunction, delegate_t>;

        template <typename delegate_t>
        constexpr inline bool hasGetPositionForChildFunction = bungeegum::detail::is_detected_exact_v<Offset, getPositionForChildFunction, delegate_t>;

        template <typename delegate_t>
        constexpr inline bool hasGetSizeFunction = bungeegum::detail::is_detected_exact_v<Size, getSizeFunction, delegate_t>;

    }

    template <typename delegate_t>
    struct SingleChildLayoutDelegate {

        SingleChildLayoutDelegate()
        {
            static_assert(detail::hasGetConstraintsForChildFunction<delegate_t>, "Error in SingleChildLayoutDelegate.hpp : type delegate_t has no valid getConstraintsForChild method");
            static_assert(detail::hasGetPositionForChildFunction<delegate_t>, "Error in SingleChildLayoutDelegate.hpp : type delegate_t has no valid getPositionForChildFunction method");
            static_assert(detail::hasGetSizeFunction<delegate_t>, "Error in SingleChildLayoutDelegate.hpp : type delegate_t has no valid getSizeFunction method");
        }

        BoxConstraints getConstraintsForChild(const BoxConstraints constraints)
        {
            return _delegate.getConstraintsForChild(constraints);
        }

        Offset getPositionForChild(const Size size, const Size childSize)
        {
            return _delegate.getPositionForChild(size, childSize);
        }

        Size getSize(const BoxConstraints constraints)
        {
            return _delegate.getSize(constraints);
        }

        SingleChildLayoutDelegate(const SingleChildLayoutDelegate& other) = default;
        SingleChildLayoutDelegate& operator=(const SingleChildLayoutDelegate& other) = default;
        SingleChildLayoutDelegate(SingleChildLayoutDelegate&& other) = default;
        SingleChildLayoutDelegate& operator=(SingleChildLayoutDelegate&& other) = default;

    private:
        delegate_t _delegate;
    };

}
}