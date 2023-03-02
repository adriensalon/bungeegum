//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <bungeegum/glue/registry.hpp>

namespace bungeegum {
namespace detail {

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

    entity_t registry::create_entity()
    {
        return _registry.create();
    }

    void registry::destroy_entity(const entity_t entity)
    {
        _registry.destroy(entity);
    }

}
}