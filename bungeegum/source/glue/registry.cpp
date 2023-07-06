#include <bungeegum/glue/registry.fwd>

namespace bungeegum {
namespace detail {

    registry_entity registry::create_entity()
    {
        return _registry.create();
    }

    void registry::destroy_entity(const registry_entity entity)
    {
        _registry.destroy(entity);
    }

}
}