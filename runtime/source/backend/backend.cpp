#include <bungeegum/backend/backend.hpp>
#include <bungeegum/core/global.fwd>

namespace bungeegum {

std::list<std::string>& hotreload_defines()
{
    if (!detail::global_manager::backend().reload_manager) {
        detail::global_manager::backend().reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::global_manager::backend().reload_defines;
}

std::list<std::filesystem::path>& hotreload_include_directories()
{
    if (!detail::global_manager::backend().reload_manager) {
        detail::global_manager::backend().reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::global_manager::backend().reload_include_directories;
}

std::list<std::filesystem::path>& hotreload_libraries()
{
    if (!detail::global_manager::backend().reload_manager) {
        detail::global_manager::backend().reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::global_manager::backend().reload_libraries;
}

std::list<std::filesystem::path>& hotreload_source_directories()
{
    if (!detail::global_manager::backend().reload_manager) {
        detail::global_manager::backend().reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::global_manager::backend().reload_source_directories;
}

std::list<std::filesystem::path>& hotreload_force_compiled_source_files()
{
    if (!detail::global_manager::backend().reload_manager) {
        detail::global_manager::backend().reload_manager = std::make_unique<detail::reloader>();
    }
    return detail::global_manager::backend().reload_force_compiled_source_files;
}
}