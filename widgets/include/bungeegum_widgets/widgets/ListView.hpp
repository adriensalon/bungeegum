#pragma once

#include <list>

#include <bungeegum_widgets/core/Axis.hpp>
#include <bungeegum_widgets/core/EdgeInsets.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that rotates its child by a integral number of quarter turns.
    /// @details Unlike Transform, which applies a transform just prior to painting, this object 
    /// applies its rotation prior to layout, which means the entire rotated box consumes only as 
    /// much space as required by the rotated child.
    struct ListView {

        /// @brief 
        /// @details 
        ListView& children(const std::vector<widget_id>& value);
        
        /// @brief
        /// @details
        ListView& padding(const EdgeInsets& value);
        
        /// @brief
        /// @details
        ListView& reverse(const bool value);
        
        /// @brief
        /// @details
        ListView& scrollDirection(const Axis value);

        /// @brief The number of clockwise quarter turns the child should be rotated.
        ListView& quarterTurns(const std::size_t value);

    private:
        friend struct access;
        void resolve(resolve_command& command);
        void draw(draw_command& command);
        std::vector<widget_id> _childrenWidgets = {};
        EdgeInsets _padding = {};
        bool _reverse = false;
        Axis _scrollDirection = Axis::vertical;
        SERIALIZE_FIELDS(_padding, _scrollDirection)
    };

}
}