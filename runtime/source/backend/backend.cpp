#include <sstream>

#include <bungeegum/backend/backend.hpp>
#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

    void backend_manager::setup_if_required()
    {
        if (!reload_manager) {
            std::wstringstream _create_reload_manager_message;
            reload_manager = std::make_unique<detail::reloader>(_create_reload_manager_message.rdbuf());
            reload_initialization_message = _create_reload_manager_message.str();
        }
    }
}

namespace hotswap {

    std::list<std::string>& defines()
    {
        detail::global().backend.setup_if_required();
        return detail::global().backend.reload_manager->defines();
    }

    std::list<std::filesystem::path>& include_directories()
    {
        detail::global().backend.setup_if_required();
        return detail::global().backend.reload_manager->include_directories();
    }

    std::list<std::filesystem::path>& libraries()
    {
        detail::global().backend.setup_if_required();
        return detail::global().backend.reload_manager->libraries();
    }

    std::list<std::filesystem::path>& source_directories()
    {
        detail::global().backend.setup_if_required();
        return detail::global().backend.reload_manager->source_directories();
    }

    std::list<std::filesystem::path>& force_compiled_source_files()
    {
        detail::global().backend.setup_if_required();
        return detail::global().backend.reload_manager->force_compiled_source_files();
    }
}
}