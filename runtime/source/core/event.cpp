#include <bungeegum/core/event.hpp>

namespace bungeegum {
namespace detail {

    events_manager::const_iterator events_manager::begin() const
    {
        return _updatables.begin();
    }

    bool events_manager::contains(const std::uintptr_t raw_event) const
    {
        return _updatables.find(raw_event) != _updatables.end();
    }

    events_manager::const_iterator events_manager::end() const
    {
        return _updatables.end();
    }

    void events_manager::notify_erase(const std::uintptr_t raw_event)
    {
        _updatables_to_erase.push_back(raw_event);
    }

    event_update_data& events_manager::operator[](const std::uintptr_t raw_event)
    {
        return _updatables[raw_event];
    }

    std::size_t events_manager::size() const
    {
        return _updatables.size();
    }

    void events_manager::update()
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t _raw_event : _updatables_to_erase) {
            _updatables.erase(_raw_event);
        }
        _updatables_to_erase.clear();
        for (std::pair<const uintptr_t, bungeegum::detail::event_update_data>& _update_data : _updatables) {
            _update_data.second.ticker();
        }
    }

}
}