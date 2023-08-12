#include <bungeegum/core/animation.hpp>

namespace bungeegum {
namespace detail {

    animations_manager::const_iterator animations_manager::begin() const
    {
        return _updatables.begin();
    }

    bool animations_manager::contains(const std::uintptr_t raw_animation) const
    {
        return _updatables.find(raw_animation) != _updatables.end();
    }

    animations_manager::const_iterator animations_manager::end() const
    {
        return _updatables.end();
    }

    void animations_manager::notify_erase(const std::uintptr_t raw_animation)
    {
        _updatables_to_erase.push_back(raw_animation);
    }

    animation_update_data& animations_manager::operator[](const std::uintptr_t raw_animation)
    {
        return _updatables[raw_animation];
    }

    std::size_t animations_manager::size() const
    {
        return _updatables.size();
    }

    void animations_manager::update(const std::chrono::milliseconds& delta_time)
    {
        // We cleanup first so that event objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t& _raw_animation : _updatables_to_erase) {
            _updatables.erase(_raw_animation);
        }
        _updatables_to_erase.clear();
        for (std::pair<const uintptr_t, bungeegum::detail::animation_update_data>& _update_data : _updatables) {
            _update_data.second.ticker(delta_time);
        }
    }
}
}