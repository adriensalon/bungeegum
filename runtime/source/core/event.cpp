#include <bungeegum/core/event.hpp>

namespace bungeegum {
namespace detail {

    events_manager::const_iterator events_manager::begin() const
    {
        return _tickables.begin();
    }

    bool events_manager::contains(const std::uintptr_t raw_event) const
    {
        return _tickables.find(raw_event) != _tickables.end();
    }

    events_manager::const_iterator events_manager::end() const
    {
        return _tickables.end();
    }

    void events_manager::notify_erase(const std::uintptr_t raw_event)
    {
        _tickables_to_erase.push_back(raw_event);
    }

    event_update_data& events_manager::operator[](const std::uintptr_t raw_event)
    {
        return _tickables[raw_event];
    }

    void events_manager::update()
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t _raw_event : _tickables_to_erase) {
            _tickables.erase(_raw_event);
        }
        _tickables_to_erase.clear();
        for (std::pair<const uintptr_t, bungeegum::detail::event_update_data>& _update_data : _tickables) {
            _update_data.second.ticker();
        }
    }

}
}