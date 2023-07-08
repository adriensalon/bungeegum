#include <bungeegum/core/interact.hpp>

namespace bungeegum {
namespace detail {

}

void interact_command::block()
{
    _data.is_blocked = true;
}

interaction_type interact_command::type() const
{
    size_t _index = _data.command_data.index();
    return static_cast<interaction_type>(_index);
}

#define get_interaction_impl(interaction_name)                                                                      \
    std::optional<interaction_name##_interaction> interact_command::interaction_name() const                        \
    {                                                                                                               \
        size_t _index = static_cast<size_t>(interaction_type::interaction_name);                                    \
        if (_index != _data.command_data.index())                                                                   \
            return std::nullopt;                                                                                    \
        detail::##interaction_name##_event _event = std::get<detail::interaction_name##_event>(_data.command_data); \
        interaction_name##_interaction _interaction;                                                                \
        _interaction._event = _event;                                                                               \
        return std::make_optional<interaction_name##_interaction>(_interaction);                                    \
    }

get_interaction_impl(window_resized);
get_interaction_impl(mouse_moved);
get_interaction_impl(mouse_down);
get_interaction_impl(mouse_up);
get_interaction_impl(mouse_pressed);

#undef get_interaction_impl

}