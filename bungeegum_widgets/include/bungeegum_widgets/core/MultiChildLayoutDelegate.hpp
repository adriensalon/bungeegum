#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    template <template <typename> typename delegate_t, typename id_t>
    struct MultiChildLayoutDelegate {

        MultiChildLayoutDelegate()
        {
            static_assert(_hasGetSizeFunction<delegate_t, id_t>, "Error in MultiChildLayoutDelegate.hpp : type delegate_t has no valid getSizeFunction method");
        }

        Size getSize(const BoxConstraints constraints)
        {
            return _delegate.getSize(constraints);
        }

        MultiChildLayoutDelegate(const MultiChildLayoutDelegate& other) = default;
        MultiChildLayoutDelegate& operator=(const MultiChildLayoutDelegate& other) = default;
        MultiChildLayoutDelegate(MultiChildLayoutDelegate&& other) = default;
        MultiChildLayoutDelegate& operator=(MultiChildLayoutDelegate&& other) = default;

    private:
        delegate_t<id_t> _delegate;

        template <typename delegateAndId_t>
        using _getSizeFunction = decltype(std::declval<delegateAndId_t>().getSize(std::declval<const BoxConstraints>()));

        template <template <typename> typename delegate_t, typename id_t>
        constexpr static bool _hasGetSizeFunction = bungeegum::detail::is_detected_exact_v<Size, _getSizeFunction, delegate_t<id_t>>;
    };
}
}