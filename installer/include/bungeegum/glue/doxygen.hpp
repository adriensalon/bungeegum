#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace bungeegum {
namespace detail {

    std::optional<std::string> doxygen_version();
}
}