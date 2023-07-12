#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/BoxConstraints.hpp>
#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    template <typename id_t>
    struct LayoutCommand {

        bool1 hasChild(const id_t& id) const
        {
        }

        void layoutChild(const id_t& id, const BoxConstraints constraints)
        {
        }

        void positionChild(const id_t& id, const Offset offset)
        {
        }
    };

    template <typename delegate_t, typename id_t>
    struct MultiChildLayoutDelegate {

        MultiChildLayoutDelegate()
        {
            static_assert(_hasGetSizeFunction<delegate_t>, "Error in MultiChildLayoutDelegate.hpp : type delegate_t has no valid getSize method");
            static_assert(_hasPerformLayoutFunction<delegate_t, id_t>, "Error in MultiChildLayoutDelegate.hpp : type delegate_t has no valid performLayout method");
        }

        Size getSize(const BoxConstraints constraints)
        {
            return _delegate.getSize(constraints);
        }

        void performLayout(const Size size, LayoutCommand<id_t>& command)
        {
            _delegate.performLayout(size, command);
        }

        MultiChildLayoutDelegate(const MultiChildLayoutDelegate& other) = default;
        MultiChildLayoutDelegate& operator=(const MultiChildLayoutDelegate& other) = default;
        MultiChildLayoutDelegate(MultiChildLayoutDelegate&& other) = default;
        MultiChildLayoutDelegate& operator=(MultiChildLayoutDelegate&& other) = default;

    private:
        delegate_t _delegate;
        LayoutCommand<id_t> _command;

        template <typename _delegate_t>
        using _getSizeFunction = decltype(std::declval<_delegate_t>().getSize(std::declval<const BoxConstraints>()));

        template <typename _delegate_t, typename _id_t>
        using _performLayoutFunction = decltype(std::declval<_delegate_t>().performLayout(std::declval<const Size>(), std::declval<LayoutCommand<_id_t>&>()));

        template <typename _delegate_t>
        constexpr static bool _hasGetSizeFunction = bungeegum::detail::is_detected_exact_v<Size, _getSizeFunction, _delegate_t>;

        template <typename _delegate_t, typename _id_t>
        constexpr static bool _hasPerformLayoutFunction = bungeegum::detail::is_detected_exact_v<void, _performLayoutFunction, _delegate_t, _id_t>;
    };
}
}