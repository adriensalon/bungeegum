#include <bungeegum/glue/registry.fwd>

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