#include <sstream>

#include <bungeegum/backend/backend.hpp>
#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

#if BUNGEEGUM_USE_HOTSWAP
    void backend_manager::setup_if_required()
    {
        if (!reload_manager) {
            std::wstringstream _create_reload_manager_message;
            reload_manager = std::make_unique<detail::reloader>(_create_reload_manager_message.rdbuf());
            reload_initialization_message = _create_reload_manager_message.str();
        }
    }

    void backend_manager::save_widgets(const std::filesystem::path& archive_path)
    {
        reloaded_saver _archiver(archive_path);
        widget_update_data& _root_update_data = global().widgets.root_update_data();
        global().widgets.traverse(_root_update_data, [&_archiver](widget_update_data& _update_data) {
            if (_update_data.saver) {
                _update_data.saver(_archiver);
            }
            // TODO for loop on properties
            return true;
        });
    }

    void backend_manager::load_widgets(const std::filesystem::path& archive_path)
    {
        reloaded_loader _archiver(archive_path);
        widget_update_data& _root_update_data = global().widgets.root_update_data();
        global().widgets.traverse(_root_update_data, [&_archiver](widget_update_data& _update_data) {
            if (_update_data.loader) {
                _update_data.loader(_archiver);
            }
            // TODO for loop on properties
            return true;
        });
    }

    // std::optional<rapidjson::Value> inspect_reloadable_widget(const widget_update_data& update_data)
    // {
    //     std::stringstream _output_stream;
    //     reloaded_saver _archiver(_output_stream);
    //     if (update_data.saver) {
    //         update_data.saver(_archiver);
    //         std::cout << _output_stream.str() << std::endl;
    //         // rapidjson::Document _document;
    //         // _document.ParseInsitu(_output_stream.str().c_str());

    //     } else {
    //         return std::nullopt;
    //     }
    // }
#endif
}

#if BUNGEEGUM_USE_HOTSWAP
std::list<std::string>& defines()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->defines();
}

std::list<std::filesystem::path>& include_directories()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->include_directories();
}

std::list<std::filesystem::path>& libraries()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->libraries();
}

std::list<std::filesystem::path>& source_directories()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->source_directories();
}

std::list<std::filesystem::path>& force_compiled_source_files()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->force_compiled_source_files();
}
#endif
}