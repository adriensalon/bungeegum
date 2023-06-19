#include <bungeegum/widgets/Title.hpp>

namespace bungeegum {
namespace widgets {

    Title& Title::color(const Color value)
    {
        bungeegum_app::color(value);
        return *this;
    }

    Title& Title::title(const std::string& value)
    {
        bungeegum_app::title(value);
        return *this;
    }

    void Title::resolve(resolve_command& command)
    {
        if (_child.has_value()) {
            float2 _childSize = command.resolve_child(_child, command.min_size(), command.max_size());
            command.position_child(_child, zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.min_size());
    }
}
}