#include <bungeegum/backend/common.fwd>
#include <bungeegum/backend/embedded.fwd>
#include <bungeegum/backend/standalone.fwd>
#include <bungeegum/core/animation.fwd>
#include <bungeegum/core/event.fwd>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/widget.fwd>

namespace bungeegum {
namespace detail {

    static animations_manager _animations;
    static backend_manager _backend;
    static events_manager _events;
    static widgets_manager _widgets;

#if BUNGEEGUM_USE_EMBEDDED
    static embedded_manager _embedded;
#endif

#if BUNGEEGUM_USE_STANDALONE
    static standalone_manager _standalone;
#endif

    animations_manager& global_manager::animations()
    {
        return _animations;
    }

    backend_manager& global_manager::backend()
    {
        return _backend;
    }

    events_manager& global_manager::events()
    {
        return _events;
    }

    widgets_manager& global_manager::widgets()
    {
        return _widgets;
    }

#if BUNGEEGUM_USE_EMBEDDED
    embedded_manager& global_manager::embedded()
    {
        return _embedded;
    }
#endif

#if BUNGEEGUM_USE_STANDALONE
    standalone_manager& global_manager::standalone()
    {
        return _standalone;
    }
#endif

}
}