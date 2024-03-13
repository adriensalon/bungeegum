#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/string.hpp>

namespace bungeegum {
namespace detail {

    static swapped_manager_data __bungeegum_swapped_manager = {};

    void setup_global_if_required()
    {
#if BUNGEEGUM_USE_HOTSWAP
		if (!__bungeegum_swapped_manager.widgets.hotswap_reloader) {
            std::wstringstream _setup_stream;
            __bungeegum_swapped_manager.widgets.hotswap_reloader = std::make_unique<detail::swapper_handle>(_setup_stream.rdbuf());
            std::string _setup_str = narrow(_setup_stream.str());
            std::cout << _setup_str << std::endl;
			// __bungeegum_global_manager.pipelines.hotswap_output.push_back(std::move(_setup_str));
        	
            // mandatory !!
            __bungeegum_swapped_manager.widgets.hotswap_reloader->set_global_data(&__bungeegum_swapped_manager);
        }
#endif
    }

#if !BUNGEEGUM_USE_HOTSWAP
    swapped_manager_data& swapped_global()
    {
        return __bungeegum_swapped_manager;
    }
#endif
}
}