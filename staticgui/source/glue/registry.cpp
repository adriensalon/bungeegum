//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/glue/registry.hpp>

namespace staticgui {
namespace glue {

    registry::registry()
    {
    }

    registry::registry(registry&& other)
    {
        *this = std::move(other);
    }

    registry& registry::operator=(registry&& other)
    {
        _registry = std::move(other._registry);
        return *this;
    }

    id_integer registry::create_entity()
    {
        return _registry.create();
    }
}
}