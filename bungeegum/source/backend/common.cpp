#include <bungeegum/backend/common.hpp>

namespace bungeegum {

std::list<std::filesystem::path>& hotreload_include_directories()
{
    if (!detail::reload_manager) {
        detail::reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::reload_include_directories;
}

std::list<std::filesystem::path>& hotreload_libraries()
{
    if (!detail::reload_manager) {
        detail::reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::reload_libraries;
}

std::list<std::filesystem::path>& hotreload_source_directories()
{
    if (!detail::reload_manager) {
        detail::reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::reload_source_directories;
}

std::list<std::filesystem::path>& hotreload_force_compiled_source_files()
{
    if (!detail::reload_manager) {
        detail::reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::reload_force_compiled_source_files;
}
}