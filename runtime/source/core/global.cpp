#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/string.hpp>

namespace bungeegum {
namespace detail {

    namespace {
        
#if BUNGEEGUM_USE_HOTSWAP

    void log_hscpp2(const std::wstring& info)
    {
        std::vector<std::string> _lines = split(narrow(info), '\n');
        for (const std::string& _line : _lines) {
            if (!_line.empty()) {
                std::size_t _index = _line.find(": ") + 2u;
                std::string _shortened = _line.substr(_index, _line.length() - _index);
                backtraced_exception _exception("Hotswap", _shortened, 0u, 0u);
                log_message(_exception);    
            }
        }
    }

#endif
    }

    static swapped_manager_data __bungeegum_swapped_manager = {};

    void setup_swapped_global_if_required()
    {
#if BUNGEEGUM_USE_HOTSWAP
		if (!__bungeegum_swapped_manager.widgets.hotswap_reloader.has_value()) {
            std::wstringstream _info_stream;
            __bungeegum_swapped_manager.widgets.hotswap_reloader.emplace(_info_stream.rdbuf());
            log_hscpp2(_info_stream.str());

        	
            // mandatory !!
            __bungeegum_swapped_manager.widgets.hotswap_reloader.set_global_data(&__bungeegum_swapped_manager);
        }
#endif
    }

#if !BUNGEEGUM_USE_HOTSWAP
    swapped_manager_data& get_swapped_global()
    {
        return __bungeegum_swapped_manager;
    }
#endif
}
}