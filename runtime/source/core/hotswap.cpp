#include <sstream>

#include <bungeegum/core/global.fwd>
#include <bungeegum/core/hotswap.hpp>
#include <bungeegum/glue/console.fwd>

namespace bungeegum {
namespace detail {

#if BUNGEEGUM_USE_HOTSWAP
    hotswap_manager_data::hotswap_manager_data()
    {
		std::wstringstream _init_msg;
		_reloader = std::make_unique<detail::reloader>(_init_msg.rdbuf());
		std::string _init_str = narrow(_init_msg.str());
		_emplace_msg(_init_msg);
    }

	void hotswap_manager_data::auto_reload(const std::filesystem::path& archive_path, std::unique_ptr<widget_update_data>& root_updatable)
	{
		std::wstringstream _update_msg;
		detail::reload_state _reload_result = _reloader->auto_update(_update_msg.rdbuf());
		if (_reload_result == detail::reload_state::started_compiling) {
			_recursive_save(archive_path, root_updatable);
		} else if (_reload_result == detail::reload_state::performed_swap) {
			_recursive_load(archive_path, root_updatable);
		}
		_emplace_msg(_update_msg);
	}

	void hotswap_manager_data::force_reload(const std::filesystem::path& archive_path, std::unique_ptr<widget_update_data>& root_updatable)
	{		
		std::wstringstream _update_msg;
		_recursive_save(archive_path, root_updatable);
		_reloader->force_update(_update_msg.rdbuf());
		_recursive_load(archive_path, root_updatable);
		_emplace_msg(_update_msg);
	}

	std::vector<std::string>& hotswap_manager_data::get_defines()
	{
		return _reloader->get_defines();
	}

	std::vector<std::filesystem::path>& hotswap_manager_data::get_include_directories()
	{
		return _reloader->get_include_directories();
	}

	std::vector<std::filesystem::path>& hotswap_manager_data::get_libraries()
	{
		return _reloader->get_libraries();
	}

	std::vector<std::filesystem::path>& hotswap_manager_data::get_source_directories()
	{
		return _reloader->get_source_directories();
	}

	std::vector<std::filesystem::path>& hotswap_manager_data::get_force_compiled_source_files()
	{
		return _reloader->get_force_compiled_source_files();
	}


    std::string hotswap_manager_data::inspect_updatable(const std::unique_ptr<widget_update_data>& updatable)
    {
        std::stringstream _output_stream;
		{
			reloaded_saver _archiver(_output_stream);
			updatable->connector.save(_archiver);
		}
		return _output_stream.str();
    }
    
	void hotswap_manager_data::patch_updatable(std::unique_ptr<widget_update_data>& updatable, const std::string& serialized)
    {
        std::stringstream _input_stream;
        _input_stream << serialized;
		{
			reloaded_loader _archiver(_input_stream);
			updatable->connector.load(_archiver);
		}
    }

	void hotswap_manager_data::register_global(void* data)
	{
		_reloader->register_global(data);
	}
	
	void hotswap_manager_data::_emplace_msg(const std::wstringstream& msg)
	{
		std::string _str = narrow(msg.str());
		if (!_str.empty()) {
			_logs.push_back(std::move(_str));
		}
	}

    void hotswap_manager_data::_recursive_load(const std::filesystem::path& archive_path, std::unique_ptr<widget_update_data>& updatable)
    {
        reloaded_loader _loader(archive_path);
		widgets_registry_data& _registry = global().widgets.registry;
        _registry.traverse_updatables(updatable, [&_loader](std::unique_ptr<widget_update_data>& _updatable) {
			_updatable->connector.load(_loader);
            return true;
        });
    }

    void hotswap_manager_data::_recursive_save(const std::filesystem::path& archive_path, std::unique_ptr<widget_update_data>& updatable)
    {
        reloaded_saver _saver(archive_path);
		widgets_registry_data& _registry = global().widgets.registry;
        _registry.traverse_updatables(updatable, [&_saver](std::unique_ptr<widget_update_data>& _updatable) {
			_updatable->connector.save(_saver);
            return true;
        });
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

    // void hotswap_manager_data::set_clean_typename(widget_update_data& update_data)
    // {
    //     update_data.resolver_command._data.clean_typename = clean_typename(update_data.kind->name());
    // }
#endif
}

#if BUNGEEGUM_USE_HOTSWAP
std::vector<std::string>& get_defines()
{
	return detail::global().hotswap.get_defines();
}

std::vector<std::filesystem::path>& get_include_directories()
{
	return detail::global().hotswap.get_include_directories();
}

std::vector<std::filesystem::path>& get_libraries()
{
	return detail::global().hotswap.get_libraries();
}

std::vector<std::filesystem::path>& get_source_directories()
{
	return detail::global().hotswap.get_source_directories();
}

std::vector<std::filesystem::path>& get_force_compiled_source_files()
{
	return detail::global().hotswap.get_force_compiled_source_files();
}
#endif
}