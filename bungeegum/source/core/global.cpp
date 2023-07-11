#include <bungeegum/backend/common.fwd>
#include <bungeegum/backend/embedded.fwd>
#include <bungeegum/backend/standalone.fwd>
#include <bungeegum/core/animation.fwd>
#include <bungeegum/core/event.fwd>
#include <bungeegum/core/global.fwd>
#include <bungeegum/core/process.fwd>
#include <bungeegum/core/widget.fwd>

namespace bungeegum {
namespace detail {

    struct all_managers {
        animations_manager animations = {};
        backend_manager backend = {};
        events_manager events = {};
        widgets_manager widgets = {};
        process_manager process = {};
    };

    static all_managers _managers = {};

#if BUNGEEGUM_USE_EMBEDDED
    static embedded_manager _embedded;
#endif

#if BUNGEEGUM_USE_STANDALONE
    static standalone_manager _standalone;
#endif

    animations_manager& global_manager::animations()
    {
        return _managers.animations;
    }

    backend_manager& global_manager::backend()
    {
        return _managers.backend;
    }

    events_manager& global_manager::events()
    {
        return _managers.events;
    }

    widgets_manager& global_manager::widgets()
    {
        return _managers.widgets;
    }

    process_manager& global_manager::process()
    {
        return _managers.process;
    }

#if BUNGEEGUM_USE_EMBEDDED
    embedded_manager& global_manager::embedded()
    {
        return _managers.embedded;
    }
#endif

#if BUNGEEGUM_USE_STANDALONE
    standalone_manager& global_manager::standalone()
    {
        return _managers.standalone;
    }
#endif

}
}