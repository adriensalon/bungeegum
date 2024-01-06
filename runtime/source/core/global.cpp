#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

    static global_manager __bungeegum_global_manager = {};

    void setup_global_if_required()
    {
#if BUNGEEGUM_USE_HOTSWAP
        __bungeegum_global_manager.hotswap.register_global(&__bungeegum_global_manager);
#endif
    }

#if !BUNGEEGUM_USE_HOTSWAP
    global_manager& global()
    {
        return __bungeegum_global_manager;
    }
#endif
}
}