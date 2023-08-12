#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief Different ways to clip a widget's content.
    enum struct Clip {

        /// @brief No clip at all.
        /// @details This is the default option for most widgets: if the content does not overflow
        /// the widget boundary, don't pay any performance cost for clipping. If the content does
        /// overflow, please explicitly specify the following Clip options:
        /// - hardEdge, which is the fastest clipping, but with lower fidelity.
        /// - antiAlias, which is a little slower than hardEdge, but with smoothed edges.
        none,

        /// @brief Clip, but do not apply anti-aliasing.
        /// @details This mode enables clipping, but curves and non-axis-aligned straight lines
        /// will be jagged as no effort is made to anti-alias. Faster than other clipping modes,
        /// but slower than none. This is a reasonable choice when clipping is needed, if the
        /// container is an axis- aligned rectangle or an axis-aligned rounded rectangle with very
        /// small corner radii.
        hardEdge,

        /// @brief Clip with anti-aliasing.
        /// @details This mode has anti-aliased clipping edges to achieve a smoother look.
        /// It' s much faster than antiAliasWithSaveLayer, but slower than hardEdge. This will be
        /// the common case when dealing with circles and arcs. Different from hardEdge and
        /// antiAliasWithSaveLayer, this clipping may have bleeding edge artifacts.
        /// (See https://fiddle.skia.org/c/21cb4c2b2515996b537f36e7819288ae for an example.)
        antiAlias,

        // we dont implement this because we dont use Flutter layers at all (requires framebuffers)
        // antiAliasWithSaveLayer
    };

}
}