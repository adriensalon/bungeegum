//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/staticgui.hpp>

namespace staticgui {
namespace internal {
    namespace impl {

        event_impl& create_event_impl()
        {
            glue::id_integer _entity = events_container.create_entity();
            return events_container.create_component<event_impl>(_entity);
        }

    }
}
}