#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

    static global_manager __bungeegum_global_manager = {};

    void setup_global()
    {
#if BUNGEEGUM_USE_HOTSWAP
        __bungeegum_global_manager.backend.setup_if_required();
        __bungeegum_global_manager.backend.reload_manager->set_global_data(&__bungeegum_global_manager);
#endif
    }

    //     global_manager& global()
    //     {
    // #if BUNGEEGUM_USE_HOTSWAP
    //         return get_global_data<global_manager>();
    // #else
    //         return __bungeegum_global_manager;
    // #endif
    //     }
}
}