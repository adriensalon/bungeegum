#include <bungeegum/backend/common.fwd>
#include <bungeegum/core/hotreload.hpp>

namespace bungeegum {

std::list<std::filesystem::path>& hotreload_include_directories()
{
    if (!detail::reloader) {
        detail::reloader = std::make_unique<detail::reload_manager>();
    }
    return detail::include_directories;
}

std::list<std::filesystem::path>& hotreload_libraries()
{
    if (!detail::reloader) {
        detail::reloader = std::make_unique<detail::reload_manager>();
    }
    return detail::libraries;
}

std::list<std::filesystem::path>& hotreload_source_directories()
{
    if (!detail::reloader) {
        detail::reloader = std::make_unique<detail::reload_manager>();
    }
    return detail::source_directories;
}

std::list<std::filesystem::path>& hotreload_force_compiled_source_files()
{
    if (!detail::reloader) {
        detail::reloader = std::make_unique<detail::reload_manager>();
    }
    return detail::force_compiled_source_files;
}
}