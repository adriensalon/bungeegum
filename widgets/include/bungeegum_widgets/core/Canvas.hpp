#pragma once

#include <bungeegum_widgets/core/BlendMode.hpp>
#include <bungeegum_widgets/core/ClipOp.hpp>
#include <bungeegum_widgets/core/Color.hpp>
#include <bungeegum_widgets/core/Image.hpp>
#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Paint.hpp>
#include <bungeegum_widgets/core/Path.hpp>
#include <bungeegum_widgets/core/Rect.hpp>

namespace bungeegum {
namespace widgets {

    struct Canvas {
        Canvas() = default;
        Canvas(const Canvas& other) = default;
        Canvas& operator=(const Canvas& other) = default;
        Canvas(Canvas&& other) = default;
        Canvas& operator=(Canvas&& other) = default;

        void clipPath(const Path& path, const bool doAntiAlias = true);

        void clipRect(const Rect rect, const ClipOp clipOp = ClipOp::intersect, const bool doAntiAlias = true);

        // void clipRRect()

        // void drawArc(const Rect& rect, const float1 startAngle, const float1 sweepAngle, const bool useCenter, )

        /// @brief Paints the given Color onto the canvas, applying the given BlendMode, with the
        /// given color being the source and the background being the destination.
        void drawColor(const Color color, const BlendMode blendMode);

        // void drawDRRect()

        /// @brief Draws the given Image into the canvas with its top-left corner at the given
        /// Offset. The image is composited into the canvas using the given Paint.
        void drawImage(const Image& image, const Offset offset, const Paint paint);

        /// @brief Draws the given Image into the canvas using the given Paint.
        /// @details The image is drawn in nine portions described by splitting the image by
        /// drawing two horizontal lines and two vertical lines, where the center argument
        /// describes the rectangle formed by the four points where these four lines intersect
        /// each other. (This forms a 3-by-3 grid of regions, the center region being described
        /// by the center argument.)
        /// The four regions in the corners are drawn, without scaling, in the four corners of
        /// the destination rectangle described by dst. The remaining five regions are drawn by
        /// stretching them to fit such that they exactly cover the destination rectangle while
        /// maintaining their relative positions.
        void drawImageNine(const Image& image, const Rect center, const Rect dst, const Paint paint);

        /// @brief Draws the subset of the given image described by the src argument into the
        /// canvas in the axis-aligned rectangle given by the dst argument.
        /// @details This might sample from outside the src rect by up to half the width of an
        /// applied filter. Multiple calls to this method with different arguments (from the
        /// same image) can be batched into a single call to drawAtlas to improve performance.
        void drawImageRect(const Image& image, const Rect src, const Rect dst, const Paint paint);

        /// @brief Draws a line between the given points using the given paint. The line is
        /// stroked, the value of the Paint.style is ignored for this call.
        /// @details The p1 and p2 arguments are interpreted as offsets from the origin.
        void drawLine(const Offset p1, const Offset p2, const Paint paint);

        /// @brief Draws an axis-aligned oval that fills the given axis-aligned rectangle with the
        /// given Paint. Whether the oval is filled or stroked (or both) is controlled by
        /// Paint.style.
        void drawOval(const Rect rect, const Paint paint);

        /// @brief Fills the canvas with the given Paint.
        /// @details To fill the canvas with a solid color and blend mode, consider drawColor
        /// instead.
        void drawPaint(const Paint paint);

        //
        //
        //
    };
}

// largest ?
}