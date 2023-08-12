#pragma once

#include <bungeegum/core/draw.hpp>
#include <bungeegum_widgets/core/BlendMode.hpp>
#include <bungeegum_widgets/core/Border.hpp>
#include <bungeegum_widgets/core/BorderRadius.hpp>
#include <bungeegum_widgets/core/BoxShadow.hpp>
#include <bungeegum_widgets/core/BoxShape.hpp>
#include <bungeegum_widgets/core/Color.hpp>
#include <bungeegum_widgets/core/DecorationImage.hpp>
#include <bungeegum_widgets/core/EdgeInsets.hpp>
#include <bungeegum_widgets/core/Gradient.hpp>


namespace bungeegum {
namespace widgets {

    /// @brief A mutable description of how to paint a box.
    /// @details The BoxDecoration class provides a variety of ways to draw a box. The box has a
    /// border, a body, and may cast a boxShadow. The shape of the box can be a circle or a
    /// rectangle. If it is a rectangle, then the borderRadius property controls the roundness of
    /// the corners. The body of the box is painted in layers. The bottom-most layer is the color,
    /// which fills the box. Above that is the gradient, which also fills the box. Finally there
    /// is the image, the precise alignment of which is controlled by the DecorationImage class.
    /// The border paints over the body; the boxShadow, naturally, paints below it.
    struct BoxDecoration {

        /// @brief The blend mode applied to the color or gradient background of the box.
        /// @details If no backgroundBlendMode is provided then the default painting blend mode is
        /// used. If no color or gradient is provided then the blend mode has no impact.
        BoxDecoration& backgroundBlendMode(const std::optional<BlendMode> value);

        /// @brief A border to draw above the background color, gradient, or image.
        /// @details Follows the shape and borderRadius. Use Border objects to describe borders
        /// that do not depend on the reading direction. Use BoxBorder objects to describe borders
        /// that should flip their left and right edges based on whether the text is being read
        /// left-to-right or right-to-left.
        std::optional<Border> border() const;

        /// @brief
        std::optional<BorderRadius> borderRadius() const;

        std::vector<BoxShadow> boxShadow() const;

        std::optional<Color> color() const;

        std::optional<Gradient> gradient() const;

        std::optional<DecorationImage> image() const;

        EdgeInsets padding() const;

        BoxShape shape() const;

    private:
        std::optional<Color> _color;
        std::optional<DecorationImage> _decoration;
        std::optional<Border> _border;
        std::optional<BorderRadius> _borderRadius;
        std::vector<BoxShadow> _boxShadow;
        std::optional<Gradient> _gradient;
        std::optional<BlendMode> _backgroundBlendMode;
        BoxShape _shape;
    };

}
}