#include <bungeegum/backend/common.fwd>
#include <bungeegum/core/hotreload.hpp>

namespace bungeegum {

std::list<std::filesystem::path>& hotreload_include_directories()
{
    return detail::include_directories;
}

std::list<std::filesystem::path>& hotreload_source_directories()
{
    return detail::source_directories;
}

}