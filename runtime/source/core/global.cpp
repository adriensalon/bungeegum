#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/string.hpp>

namespace bungeegum {
namespace detail {

    static global_manager_data __bungeegum_global_manager = {};

    void setup_global_if_required()
    {
#if BUNGEEGUM_USE_HOTSWAP
		if (!__bungeegum_global_manager.widgets.hotswap_reloader) {
            std::wstringstream _setup_stream;
            __bungeegum_global_manager.widgets.hotswap_reloader = std::make_unique<detail::reloader>(_setup_stream.rdbuf());
            std::string _setup_str = narrow(_setup_stream.str());
			__bungeegum_global_manager.logs.hotswap_output.push_back(std::move(_setup_str));
        	__bungeegum_global_manager.widgets.hotswap_reloader->set_global_data(&__bungeegum_global_manager);
        }
#endif
    }

#if !BUNGEEGUM_USE_HOTSWAP
    global_manager_data& global()
    {
        return __bungeegum_global_manager;
    }
#endif
}
}