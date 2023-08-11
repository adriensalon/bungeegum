#pragma once

#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Rect.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A complex, one-dimensional subset of a plane.
    /// @details A path consists of a number of sub-paths, and a current point. Sub-paths consist
    /// of segments of various types, such as lines, arcs, or beziers. Sub-paths can be open or
    /// closed, and can self-intersect. Closed sub-paths enclose a (possibly discontiguous) region
    /// of the plane based on the current fillType. The current point is initially at the origin.
    /// After each operation adding a segment to a sub-path, the current point is updated to the
    /// end of that segment. Paths can be drawn on canvases using Canvas.drawPath, and can used
    /// to create clip regions using Canvas.clipPath.
    struct Path {
        Path() = default;
        Path(const Path& other) = default;
        Path& operator=(const Path& other) = default;
        Path(Path&& other) = default;
        Path& operator=(Path&& other) = default;

        /// @brief Adds a new sub-path with one arc segment that consists of the arc that follows
        /// the edge of the oval bounded by the given rectangle, from startAngle radians around
        /// the oval up to startAngle + sweepAngle radians around the oval, with zero radians being
        /// the point on the right hand side of the oval that crosses the horizontal line that
        /// intersects the center of the rectangle and with positive angles going clockwise around
        /// the oval.
        void addArc(const Rect& oval, const float1 startAngle, const float1 sweepAngle);

        /// @brief Adds a new sub-path that consists of a curve that forms the ellipse that fills
        /// the given rectangle.
        /// @details To add a circle, pass an appropriate rectangle as oval. Rect.fromCircle can
        /// be used to easily describe the circle's center Offset and radius.
        void addOval(const Rect& oval);

        /// @brief Adds the sub-paths of path, offset by offset, to this path.
        /// @details If matrix4 is specified, the path will be transformed by this matrix after the
        /// matrix is translated by the given offset. The matrix is a 4x4 matrix stored in column
        /// major order.
        void addPath(const Path& path, const Offset& offset, const std::optional<float4x4>& matrix4 = std::nullopt);

        /// @brief Adds a new sub-path with a sequence of line segments that connect the given
        /// points.
        /// @details If close is true, a final line segment will be added that connects the last
        /// point to the first point. The points argument is interpreted as offsets from the origin.
        void addPolygon(const std::vector<Offset>& points, const bool close);

        /// @brief Adds a new sub-path that consists of four lines that outline the given rectangle.
        void addRect(const Rect& rect);

        // void addRRect(const RRect& rrect);

        void arcTo(const Rect& rect, const float1 startAngle, const float1 sweepAngle, const bool forceMoveTo);

        // void arcToPoint(const Offset& arcEnd, )

        void close();

        Path shift(const Offset& offset);

        Path transform(const float4x4 matrix4);

    private:
    };
}

// largest ?
}