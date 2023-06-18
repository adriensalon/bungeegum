#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/misc/Alignment.hpp>
#include <bungeegum/widgets/misc/AlignmentDirectional.hpp>
#include <bungeegum/widgets/misc/BoxConstraints.hpp>
#include <bungeegum/widgets/misc/BoxDecoration.hpp>
#include <bungeegum/widgets/misc/Color.hpp>
#include <bungeegum/widgets/misc/EdgeInsets.hpp>
#include <bungeegum/widgets/misc/EdgeInsetsDirectional.hpp>
#include <bungeegum/widgets/misc/ShapeDecoration.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A convenience widget that combines common painting, positioning, and sizing widgets.
    /// @details A container first surrounds the child with padding (inflated by any borders
    /// present in the decoration) and then applies additional constraints to the padded extent
    /// (incorporating the width and height as constraints, if either is non-null). The container
    /// is then surrounded by additional empty space described from the margin.
    /// During painting, the container first applies the given transform, then paints the
    /// decoration to fill the padded extent, then it paints the child, and finally paints the
    /// foregroundDecoration, also filling the padded extent.
    /// Containers with no children try to be as big as possible unless the incoming constraints
    /// are unbounded, in which case they try to be as small as possible. Containers with children
    /// size themselves to their children. The width, height, and constraints properties
    /// override this.
    /// By default, containers return false for all hit tests. If the color property is specified,
    /// the hit testing is handled by ColoredBox, which always returns true. If the decoration or
    /// foregroundDecoration properties are specified, hit testing is handled by Decoration.hitTest.
    struct Container {

        /// @brief Align the child within the container.
        Container& alignment(const Alignment& value);

        /// @brief Align the child within the container.
        Container& alignment(const AlignmentDirectional& value);

        /// @brief The child contained by the container.
        template <typename child_widget_t>
        Container& child(child_widget_t& value)
        {
            adopt(this, value);
            return *this;
    }

        /// @brief The color to paint behind the child.
        /// @details This property should be preferred when the background is a simple color. For
        /// other cases, such as gradients or images, use the decoration property. If the
        /// decoration is used, this property will not.
        Container& color(const Color value);

        /// @brief Additional constraints to apply to the child.
        Container& constraints(const BoxConstraints& value);

        /// @brief The decoration to paint behind the child.
        Container& decoration(const BoxDecoration& value);

        /// @brief The decoration to paint behind the child.
        Container& decoration(const ShapeDecoration& value);

        /// @brief The decoration to paint in front of the child.
        Container& foregroundDecoration(const BoxDecoration& value);

        /// @brief The decoration to paint in front of the child.
        Container& foregroundDecoration(const ShapeDecoration& value);

        /// @brief
        Container& height(const float1 value);

        /// @brief Empty space to surround the decoration and child.
        Container& margin(const EdgeInsets& value);

        /// @brief Empty space to surround the decoration and child.
        Container& margin(const EdgeInsetsDirectional& value);

        /// @brief Empty space to inscribe inside the decoration. The child, if any, is placed
        /// inside this padding.
        Container& padding(const EdgeInsets& value);

        /// @brief Empty space to inscribe inside the decoration. The child, if any, is placed
        /// inside this padding.
        Container& padding(const EdgeInsetsDirectional& value);

        /// @brief The transformation matrix to apply before painting the container.
        Container& transform(const float4x4 value);

        /// @brief The alignment of the origin, relative to the size of the container, if
        /// transform is specified.
        Container& transformAlignment(const Alignment& value);

        /// @brief The alignment of the origin, relative to the size of the container, if
        /// transform is specified.
        Container& transformAlignment(const AlignmentDirectional& value);

        /// @brief
        Container& width(const float1 value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        void draw(draw_command& command);

        Color _color = { 0xFF000000 };
        std::optional<adopted_widget> _adoptedChild = std::nullopt;
        std::optional<BoxConstraints> _constraints = std::nullopt;
        std::optional<float2> _size = std::nullopt;
        std::optional<float4x4> _transform = std::nullopt;
        std::optional<std::variant<Alignment, AlignmentDirectional>> _alignment = std::nullopt;
        std::optional<std::variant<Alignment, AlignmentDirectional>> _transformAlignment = std::nullopt;
        std::optional<std::variant<BoxDecoration, ShapeDecoration>> _decoration = std::nullopt;
        std::optional<std::variant<BoxDecoration, ShapeDecoration>> _foregroundDecoration = std::nullopt;
        std::optional<std::variant<EdgeInsets, EdgeInsetsDirectional>> _margin = std::nullopt;
        std::optional<std::variant<EdgeInsets, EdgeInsetsDirectional>> _padding = std::nullopt;
    };
}
}