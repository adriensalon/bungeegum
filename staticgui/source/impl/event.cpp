//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/staticgui.hpp>
#include <staticgui/utils/ecs.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        event_impl& create_event_impl()
        {
            ecs::entity _entity(events_container);
            return _entity.create_component<event_impl>();
        }

    }
}
}