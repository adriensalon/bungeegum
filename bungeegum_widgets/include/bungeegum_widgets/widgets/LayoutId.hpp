#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    template <typename idValue_t>
    struct LayoutId {

        LayoutId& id(const idValue_t& value);

        LayoutId& id(idValue_t&& value);

    private:
        friend struct access;
        void resolve(resolve_command& command);

        std::optional<idValue_t> _value = std::nullopt;
    };

}
}