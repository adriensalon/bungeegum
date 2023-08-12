#include <bungeegum_widgets/widgets/Title.hpp>

namespace bungeegum {
namespace widgets {

    Title& Title::color(const Color value)
    {
        standalone_app::color(value);
        return *this;
    }

    Title& Title::title(const std::string& value)
    {
        standalone_app::title(value);
        return *this;
    }

    void Title::resolve(resolve_command& command)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        if (_childWidget.has_value()) {
            float2 _childSize = command.resolve_child(_childWidget.value(), command.min_size(), command.max_size());
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.max_size());
    }
}
}