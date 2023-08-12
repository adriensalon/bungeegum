#pragma once

namespace bungeegum {
namespace widgets {

    /// @brief How to behave during hit tests.
    enum struct HitTestBehavior {

        /// @brief Targets that defer to their children receive events within their bounds only
        /// if one of their children is hit by the hit test.
        deferToChild,

        /// @brief Opaque targets can be hit by hit tests, causing them to both receive events
        /// within their bounds and prevent targets visually behind them from also receiving
        /// events.
        opaque,

        /// @brief Translucent targets both receive events within their bounds and permit targets
        /// visually behind them to also receive events.
        translucent
    };
}

}