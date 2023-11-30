#pragma once

#include <bungeegum/core/runtime.hpp>
#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A scrollable list of widgets arranged linearly.
    struct ListView {


    private:
        friend struct access;
        void resolve(resolve_command& command);

    };

}
}