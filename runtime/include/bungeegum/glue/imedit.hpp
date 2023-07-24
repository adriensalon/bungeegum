#pragma once

namespace bungeegum {
namespace detail {

    template <typename value_t>
    void edit_field(const std::string& name, value_t& value);
}
}

#include <bungeegum/glue/imedit.inl>