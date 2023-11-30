#pragma once

#include <bungeegum/core/curve.hpp>
#include <bungeegum/core/widget.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief
    template <typename animatedValue_t>
    struct TweenAnimationBuilder {

        /// @brief
        using ValueWidgetBuilder = std::function<widget_id(const animatedValue_t& value, std::optional<widget_id>& child)>;

        /// @brief Called every time the animation value changes.
        TweenAnimationBuilder& builder(const ValueWidgetBuilder& value)
        {
            _animation.on_value_changed([this](const animatedValue_t& valueChanged) {
                value(valueChanged, _child);
                must_resolve(this);
            });
            return *this;
        }

        /// @brief The child widget to pass to the builder.
        TweenAnimationBuilder& child(const std::optional<widget_id>& value)
        {
            if (_child.has_value()) {
                abandon(this, _child.value());
            }
            _child = value;
            return *this;
        }

        /// @brief The duration over which to animate the parameters of this container.
        template <typename durationUnit_t = std::chrono::seconds>
        TweenAnimationBuilder& duration(const unsigned int value)
        {
            _animation.duration<durationUnit_t>(value);
            return *this;
        }

        /// @brief
        TweenAnimationBuilder& onEnd(const std::function<void()>& value)
        {
            _animation.on_end(value);
        }

        /// @brief The curve to apply when animating the parameters of this container.
        TweenAnimationBuilder& shape(const curve& value)
        {
            _animation.shape(value);
            return *this;
        }

        /// @brief
        TweenAnimationBuilder& tween(const animation<animatedValue_t>& value)
        {
            _animation = value;
            return *this;
        }

    private:
        friend struct access;
        void resolve(resolve_command& command)
        {
            if (_child.has_value()) {
                float2 _childSize = command.resolve_child(_child.value(), command.min_size(), command.max_size());
                command.position_child(_child.value(), zero<float2>);
                command.resize(_childSize);
            } else {
                command.resize(command.max_size());
            }
        }

        animation<animatedValue_t> _animation = {};
        std::optional<widget_id> _child = std::nullopt;
    };

}
}