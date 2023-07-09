#include <bungeegum/core/animation.hpp>

namespace bungeegum {
namespace detail {

    animations_manager::const_iterator animations_manager::begin() const
    {
        return _tickables.begin();
    }

    bool animations_manager::contains(const std::uintptr_t& raw_event) const
    {
        return _tickables.find(raw_event) != _tickables.end();
    }

    animations_manager::const_iterator animations_manager::end() const
    {
        return _tickables.end();
    }

    void animations_manager::notify_erase(const std::uintptr_t& raw_event)
    {
        _tickables_to_erase.push_back(raw_event);
    }

    animation_update_data& animations_manager::operator[](const std::uintptr_t& raw_event)
    {
        return _tickables[raw_event];
    }

    void animations_manager::update(const std::chrono::milliseconds& delta_time)
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t& _raw_event : _tickables_to_erase) {
            _tickables.erase(_raw_event);
        }
        _tickables_to_erase.clear();
        for (std::pair<const uintptr_t, bungeegum::detail::animation_update_data>& _update_data : _tickables) {
            _update_data.second.ticker(delta_time);
        }
    }
}
}