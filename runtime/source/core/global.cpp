#include <bungeegum/core/global.fwd>
#include <bungeegum/glue/string.hpp>

namespace bungeegum {
namespace detail {

    static swapped_manager_data __bungeegum_swapped_manager = {};

    void setup_swapped_global_if_required()
    {
#if BUNGEEGUM_USE_HOTSWAP
		if (!__bungeegum_swapped_manager.widgets.hotswap_reloader.has_value()) {
            std::wstringstream _info_stream;
            __bungeegum_swapped_manager.widgets.hotswap_reloader.emplace(_info_stream.rdbuf());
            std::string _info = narrow(_info_stream.str());
            std::cout << _info;
			// __bungeegum_global_manager.pipelines.hotswap_output.push_back(std::move(_setup_str));
        	
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