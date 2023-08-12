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

    std::optional<std::string> backend_manager::inspect_reloadable_widget(const widget_update_data& update_data)
    {
        std::stringstream _output_stream;
        if (update_data.saver) {
            {
                reloaded_saver _archiver(_output_stream);
                update_data.saver(_archiver);
            }
            return _output_stream.str();
        } else {
            return std::nullopt;
        }
    }

    void backend_manager::update_reloadable_widget(widget_update_data& update_data, const std::string& serialized)
    {
        std::stringstream _input_stream;
        _input_stream << serialized;
        if (update_data.loader) {
            {
                reloaded_loader _archiver(_input_stream);
                update_data.loader(_archiver);
            }
        }
    }
#endif

#if BUNGEEGUM_USE_OVERLAY
    std::string clean_typename(const std::string& raw_typename)
    {
        std::size_t _last_space = raw_typename.find_last_of(' ');
        if (_last_space == raw_typename.npos) {
            _last_space = 0;
        }
        std::size_t _last_column = raw_typename.find_last_of(':');
        if (_last_column == raw_typename.npos) {
            _last_column = 0;
        }
        std::size_t _offset = std::max(_last_space, _last_column) + 1;
        std::size_t _length = raw_typename.length() - _offset;
        return raw_typename.substr(_offset, _length);
    }

    // void backend_manager::set_clean_typename(widget_update_data& update_data)
    // {
    //     update_data.resolver_command._data.clean_typename = clean_typename(update_data.kind->name());
    // }
#endif
}

#if BUNGEEGUM_USE_HOTSWAP
std::vector<std::string>& defines()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->defines();
}

std::vector<std::filesystem::path>& include_directories()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->include_directories();
}

std::vector<std::filesystem::path>& libraries()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->libraries();
}

std::vector<std::filesystem::path>& source_directories()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->source_directories();
}

std::vector<std::filesystem::path>& force_compiled_source_files()
{
    detail::setup_global_if_required();
    return detail::global().backend.reload_manager->force_compiled_source_files();
}
#endif
}