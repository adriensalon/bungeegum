#pragma once

#include <bungeegum_widgets/core/BoxFit.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief How a box should be inscribed into another box.
    /// @details See also applyBoxFit, which applies the sizing semantics of these values (though
    /// not the alignment semantics).
    enum struct BoxFit {

        /// @brief Fill the target box by distorting the source's aspect ratio.
        fill,

        /// @brief As large as possible while still containing the source entirely within the
        /// target box.
        contain,

        /// @brief As small as possible while still covering the entire target box.
        /// @details To actually clip the content, use clipBehavior: Clip.hardEdge alongside this
        /// in a FittedBox.
        cover,

        /// @brief Make sure the full width of the source is shown, regardless of whether this
        /// means the source overflows the target box vertically.
        /// @details To actually clip the content, use clipBehavior: Clip.hardEdge alongside this
        /// in a FittedBox.
        fitWidth,

        /// @brief Make sure the full height of the source is shown, regardless of whether this
        /// means the source overflows the target box horizontally.
        /// @details To actually clip the content, use clipBehavior: Clip.hardEdge alongside this
        /// in a FittedBox.
        fitHeight,

        /// @brief Align the source within the target box (by default, centering) and discard any
        /// portions of the source that lie outside the box.
        /// @details The source image is not resized. To actually clip the content, use
        /// clipBehavior: Clip.hardEdge alongside this in a FittedBox.
        none,

        /// @brief Align the source within the target box (by default, centering) and discard any
        /// portions of the source that lie outside the box.
        /// @details The source image is not resized. To actually clip the content, use
        /// clipBehavior: Clip.hardEdge alongside this in a FittedBox.
        scaleDown,
    };

    /// @brief The pair of sizes returned by applyBoxFit.
    struct FittedSizes {
        FittedSizes() = delete;
        FittedSizes(const FittedSizes& other) = default;
        FittedSizes& operator=(const FittedSizes& other) = default;
        FittedSizes(FittedSizes&& other) = default;
        FittedSizes& operator=(FittedSizes&& other) = default;

        /// @brief Creates an object to store a pair of sizes, as would be returned by applyBoxFit.
        FittedSizes(const Size source, const Size destination);

        /// @brief The size of the part of the input to show on the output.
        Size source() const;

        /// @brief The size of the part of the output on which to show the input.
        Size destination() const;

    private:
        float2 _sourceSize = {};
        float2 _destinationSize = {};
    };

    /// @brief Apply a BoxFit value. The arguments to this method, in addition to the BoxFit value
    /// to apply, are two sizes, ostensibly the sizes of an input box and an output box.
    /// Specifically, the inputSize argument gives the size of the complete source that is being
    /// fitted, and the outputSize gives the size of the rectangle into which the source is to be
    /// drawn.
    /// @details This function then returns two sizes, combined into a single FittedSizes object.
    /// The FittedSizes.source size is the subpart of the inputSize that is to be shown. If the
    /// entire input source is shown, then this will equal the inputSize, but if the input source
    /// is to be cropped down, this may be smaller. The FittedSizes.destination size is the subpart
    /// of the outputSize in which to paint the (possibly cropped) source. If the
    /// FittedSizes.destination size is smaller than the outputSize then the source is being
    /// letterboxed (or pillarboxed). This method does not express an opinion regarding the
    /// alignment of the source and destination sizes within the input and output rectangles.
    /// Typically they are centered (this is what BoxDecoration does, for instance, and is how
    /// BoxFit is defined). The Alignment class provides a convenience function,
    /// Alignment.inscribe, for resolving the sizes to rects.
    FittedSizes applyBoxFit(const BoxFit fit, const Size source, const Size destination);

}
}