#include <sstream>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/hotswap.hpp>

namespace bungeegum {
namespace detail {

#if BUNGEEGUM_USE_HOTSWAP
    void hotswap_manager::setup_if_required()
    {
        if (!reload_manager) {
            std::wstringstream _create_reload_manager_message;
            reload_manager = std::make_unique<detail::reloader>(_create_reload_manager_message.rdbuf());
            reload_initialization_message = _create_reload_manager_message.str();
        }
    }

    void hotswap_manager::save_widgets(const std::filesystem::path& archive_path, const widget_update_data& root_updatable)
    {
        reloaded_saver _archiver(archive_path);
		std::function<void(const widget_update_data&)> _traverse_function = [&_archiver, &_traverse_function] (const widget_update_data& _updatable) {
			if (_updatable.saver) {
                _updatable.saver(_archiver);
            }
			for (const widget_update_data& _child_updatable : _updatable.children) {
				_traverse_function(_child_updatable);
			}
		};
		_traverse_function(root_updatable);
    }

    void hotswap_manager::load_widgets(const std::filesystem::path& archive_path, const widget_update_data& root_updatable)
    {
        reloaded_loader _archiver(archive_path);
		std::function<void(const widget_update_data&)> _traverse_function = [&_archiver, &_traverse_function] (const widget_update_data& _updatable) {
			if (_updatable.loader) {
                _updatable.loader(_archiver);
            }
			for (const widget_update_data& _child_updatable : _updatable.children) {
				_traverse_function(_child_updatable);
			}
		};
		_traverse_function(root_updatable);
    }

    std::optional<std::string> hotswap_manager::inspect_reloadable_widget(const widget_update_data& update_data)
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

    void hotswap_manager::update_reloadable_widget(widget_update_data& update_data, const std::string& serialized)
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

    // void hotswap_manager::set_clean_typename(widget_update_data& update_data)
    // {
    //     update_data.resolver_command._data.clean_typename = clean_typename(update_data.kind->name());
    // }
#endif
}

#if BUNGEEGUM_USE_HOTSWAP
namespace hotswap {

    std::vector<std::string>& get_defines()
    {
        detail::setup_global_if_required();
        return detail::global().hotswap.reload_manager->defines();
    }

    std::vector<std::filesystem::path>& get_include_directories()
    {
        detail::setup_global_if_required();
        return detail::global().hotswap.reload_manager->include_directories();
    }

    std::vector<std::filesystem::path>& get_libraries()
    {
        detail::setup_global_if_required();
        return detail::global().hotswap.reload_manager->libraries();
    }

    std::vector<std::filesystem::path>& get_source_directories()
    {
        detail::setup_global_if_required();
        return detail::global().hotswap.reload_manager->source_directories();
    }

    std::vector<std::filesystem::path>& get_force_compiled_source_files()
    {
        detail::setup_global_if_required();
        return detail::global().hotswap.reload_manager->force_compiled_source_files();
    }

}
#endif
}