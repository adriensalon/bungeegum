#include <bungeegum/core/animation.hpp>

namespace bungeegum {
namespace detail {

    void animation_container_data::notify_erase(const std::uintptr_t& raw_animation)
    {
        _tickables_to_erase.push_back(raw_animation);
    }

    void animation_container_data::tick(const std::chrono::milliseconds& delta_time)
    {
        // We cleanup first so that animation objects whose lifetimes have expired
        // are not iterated in the next step.
        for (const std::uintptr_t _raw_animation : _tickables_to_erase) {
            tickables.erase(_raw_animation);
        }
        for (detail::animation_update_data& _update_data : tickables) {
            _update_data.ticker(delta_time);
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