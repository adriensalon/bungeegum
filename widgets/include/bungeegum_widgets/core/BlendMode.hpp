#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Algorithms to use when painting on the canvas.
    /// @details When drawing a shape or image onto a canvas, different algorithms can be used to
    /// blend the pixels. The different values of BlendMode specify different such algorithms.
    /// Each algorithm has two inputs, the source, which is the image being drawn, and the
    /// destination, which is the image into which the source image is being composited. The
    /// destination is often thought of as the background. The source and destination both have
    /// four color channels, the red, green, blue, and alpha channels. These are typically
    /// represented as numbers in the range 0.f to 1.f. The output of the algorithm also has these
    /// same four channels, with values computed from the source and destination.
    enum struct BlendMode {

        /// @brief Drop both the source and destination images, leaving nothing.
        /// @details This corresponds to the "clear" Porter-Duff operator.
        clear,

        /// @brief Drop the destination image, only paint the source image.
        /// @details Conceptually, the destination is first cleared, then the source image is
        /// painted. This corresponds to the "Copy" Porter-Duff operator.
        src,

        /// @brief Drop the source image, only paint the destination image.
        /// @details Conceptually, the source image is discarded, leaving the destination
        /// untouched. This corresponds to the "Destination" Porter-Duff operator.
        dst,

        /// @brief Composite the source image over the destination image.
        /// @details This is the default value. It represents the most intuitive case, where shapes
        /// are painted on top of what is below, with transparent areas showing the destination
        /// layer. This corresponds to the "Source over Destination" Porter-Duff operator, also
        /// known as the Painter's Algorithm.
        srcOver,

        /// @brief Composite the source image under the destination image.
        /// @details This is the opposite of srcOver. This corresponds to the "Destination over
        /// Source" Porter-Duff operator. This is useful when the source image should have been
        /// painted before the destination image, but could not be.
        dstOver,

        /// @brief Show the source image, but only where the two images overlap. The destination
        /// image is not rendered, it is treated merely as a mask. The color channels of the
        /// destination are ignored, only the opacity has an effect.
        /// @details To show the destination image instead, consider dstIn. To reverse the semantic
        /// of the mask (only showing the source where the destination is absent, rather than where
        /// it is present), consider srcOut. This corresponds to the "Source in Destination"
        /// Porter-Duff operator.
        srcIn,

        /// @brief Show the destination image, but only where the two images overlap. The source
        /// image is not rendered, it is treated merely as a mask. The color channels of the source
        /// are ignored, only the opacity has an effect.
        /// @details To show the source image instead, consider srcIn. To reverse the semantic of
        /// the mask (only showing the source where the destination is present, rather than where it
        /// is absent), consider dstOut. This corresponds to the "Destination in Source" Porter-Duff
        /// operator.
        dstIn,

        /// @brief Show the source image, but only where the two images do not overlap. The
        /// destination image is not rendered, it is treated merely as a mask. The color channels
        /// of the destination are ignored, only the opacity has an effect.
        /// @details To show the destination image instead, consider dstOut. To reverse the
        /// semantic of the mask (only showing the source where the destination is present, rather
        /// than where it is absent), consider srcIn. This corresponds to the "Source out
        /// Destination" Porter-Duff operator.
        srcOut,

        /// @brief Show the destination image, but only where the two images do not overlap. The
        /// source image is not rendered, it is treated merely as a mask. The color channels of the
        /// source are ignored, only the opacity has an effect.
        /// @details To show the source image instead, consider srcOut. To reverse the semantic of
        /// the mask (only showing the destination where the source is present, rather than where it
        /// is absent), consider dstIn. This corresponds to the "Destination out Source" Porter-Duff
        /// operator.
        dstOut,

        /// @brief Composite the source image over the destination image, but only where it overlaps
        /// the destination.
        /// @details This corresponds to the "Source atop Destination" Porter-Duff operator. This is
        /// essentially the srcOver operator, but with the output's opacity channel being set to
        /// that of the destination image instead of being a combination of both image's opacity
        /// channels. For a variant with the destination on top instead of the source, see dstATop.
        srcATop,

        /// @brief Composite the destination image over the source image, but only where it overlaps
        /// the source.
        /// @details This corresponds to the "Destination atop Source" Porter-Duff operator. This is
        /// essentially the dstOver operator, but with the output's opacity channel being set to
        /// that of the source image instead of being a combination of both image's opacity
        /// channels. For a variant with the source on top instead of the destination, see srcATop.
        dstATop,

        /// @brief Apply a bitwise xor operator to the source and destination images. This leaves
        /// transparency where they would overlap.
        /// @details This corresponds to the "Source xor Destination" Porter-Duff operator.
        xor,

        /// @brief Sum the components of the source and destination images.
        /// @details Transparency in a pixel of one of the images reduces the contribution of that
        /// image to the corresponding output pixel, as if the color of that pixel in that image
        /// was darker. This corresponds to the "Source plus Destination" Porter-Duff operator.
        plus,

        // /// @brief
        // /// @details
        // modulate,

        // /// @brief
        // /// @details
        // screen,

        // /// @brief
        // /// @details
        // overlay,

        // /// @brief
        // /// @details
        // darken,

        // /// @brief
        // /// @details
        // lighten,

        // /// @brief
        // /// @details
        // colorDodge,

        // /// @brief
        // /// @details
        // colorBurn,

        // /// @brief
        // /// @details
        // hardLight,

        // /// @brief
        // /// @details
        // softLight,

        // /// @brief
        // /// @details
        // difference,

        // /// @brief
        // /// @details
        // exclusion,

        // /// @brief
        // /// @details
        // multiply,

        // /// @brief
        // /// @details
        // hue,

        // /// @brief
        // /// @details
        // saturation,

        // /// @brief
        // /// @details
        // color,

        // /// @brief
        // /// @details
        // luminosity,

    };

}
}