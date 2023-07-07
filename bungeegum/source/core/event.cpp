#include <bungeegum/core/event.hpp>

namespace bungeegum {
namespace detail {

    void event_container_data::notify_erase(const std::uintptr_t& raw_event)
    {
        _tickables_to_erase.push_back(raw_event);
    }

    void event_container_data::tick()
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t _raw_event : _tickables_to_erase) {
            tickables.erase(_raw_event);
        }
        for (detail::event_update_data& _update_data : tickables) {
            _update_data.ticker();
        }
        if (_tickables_to_erase.size() > 1) {
            std::sort(
                _tickables_to_erase.begin(),
                _tickables_to_erase.end(),
                std::greater {});
        }
        _tickables_to_erase.clear();
    }

}
}