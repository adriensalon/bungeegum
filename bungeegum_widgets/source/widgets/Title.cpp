#include <bungeegum_widgets/widgets/Title.hpp>

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
            float2 _childSize = command.resolve_child(_child.value(), command.min_size(), command.max_size());
            command.position_child(_child.value(), zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.max_size());
    }
}
}