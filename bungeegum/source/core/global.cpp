#include <bungeegum/backend/backend.fwd>
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
        // embedded_manager embedded = {};
        standalone_manager standalone = {};
    };

    static all_managers _managers = {};

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

    //     embedded_manager& global_manager::embedded()
    //     {
    //         return _managers.embedded;
    //     }

    standalone_manager& global_manager::standalone()
    {
        return _managers.standalone;
    }
}
}