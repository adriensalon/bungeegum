#pragma once

#include <any>

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/SingleChildLayoutDelegate.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that defers the layout of its single child to a delegate.
    /// @details The delegate can determine the layout constraints for the child and can decide
    /// where to position the child. The delegate can also determine the size of the parent, but
    /// the size of the parent cannot depend on the size of the child.
    struct CustomSingleChildLayout {

        /// @brief The widget below this widget in the tree.
        template <typename childWidget_t>
        CustomSingleChildLayout& child(childWidget_t& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        /// @brief The delegate that controls the layout of the child.
        template <typename delegate_t, typename... delegate_args_t>
        CustomSingleChildLayout& delegate(delegate_args_t&&... args)
        {
            using _delegateType_t = SingleChildLayoutDelegate<delegate_t>;
            _delegate.untyped = std::make_any<_delegateType_t>(std::forward<delegate_args_t>(args)...);
            _delegate.getConstraintsForChild = [this](const BoxConstraints constraints) {
                _delegateType_t& _typedDelegate = std::any_cast<_delegateType_t&>(_delegate.untyped);
                return _typedDelegate.getConstraintsForChild(constraints);
            };
            _delegate.getPositionForChild = [this](const Size size, const Size childSize) {
                _delegateType_t& _typedDelegate = std::any_cast<_delegateType_t&>(_delegate.untyped);
                return _typedDelegate.getPositionForChild(size, childSize);
            };
            _delegate.getSize = [this](const BoxConstraints constraints) {
                _delegateType_t& _typedDelegate = std::any_cast<_delegateType_t&>(_delegate.untyped);
                return _typedDelegate.getSize(constraints);
            };
            return *this;
        }

    private:
        friend struct access;
        void resolve(resolve_command& command);

        struct untypedDelegate {
            std::any untyped = nullptr;
            std::function<BoxConstraints(const BoxConstraints constraints)> getConstraintsForChild = nullptr;
            std::function<Offset(const Size size, const Size childSize)> getPositionForChild = nullptr;
            std::function<Size(const BoxConstraints constraints)> getSize = nullptr;
        };

        std::optional<runtime_widget> _childWidget = std::nullopt;
        untypedDelegate _delegate = {};
    };

}
}