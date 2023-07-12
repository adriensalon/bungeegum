#pragma once

#include <any>
#include <initializer_list>

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/MultiChildLayoutDelegate.hpp>
#include <bungeegum_widgets/widgets/LayoutId.hpp>

namespace bungeegum {
namespace widgets {

    template <typename id_t = LayoutIdDefaultType>
    struct CustomMultiChildLayout {

        CustomMultiChildLayout& children(std::initializer_list<std::reference_wrapper<LayoutId<id_t>>> value)
        {
            for (const runtime_widget& _widget : _childrenWidgets) {
                abandon(this, _widget);
            }
            _childrenWidgets.clear();
            for (std::reference_wrapper<LayoutId<id_t>> _widget : value) {
                _childrenWidgets.push_back(runtime_widget(_widget.get()));
                adopt(this, _widget);
            }
            return *this;
        }

        template <typename delegate_t, typename... delegate_args_t>
        CustomMultiChildLayout& delegate(delegate_args_t&&... args)
        {
            using _delegateType_t = MultiChildLayoutDelegate<delegate_t, id_t>;
            _delegate.untyped = std::make_any<_delegateType_t>(std::forward<delegate_args_t>(args)...);
            _delegate.getSize = [this](const BoxConstraints constraints) {
                _delegateType_t& _typedDelegate = std::any_cast<_delegateType_t&>(_delegate.untyped);
                return _typedDelegate.getSize(constraints);
            };

            // perform layout

            return *this;
        }

    private:
        friend struct access;
        void resolve(resolve_command& command)
        {
            // TODO
        }

        struct untypedDelegate {
            std::any untyped = nullptr;
            std::function<Size(const BoxConstraints constraints)> getSize = nullptr;

            // perform layout
        };

        std::vector<runtime_widget> _childrenWidgets = {};
        untypedDelegate _delegate = {};
    };

}
}