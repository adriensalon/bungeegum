#include <bungeegum_widgets/widgets/RotatedBox.hpp>

namespace bungeegum {
namespace widgets {

    RotatedBox& RotatedBox::child(const widget_id& value)
    {
        if (_childWidget.has_value())
            abandon(this, _childWidget.value());
        _childWidget = value;
        adopt(this, _childWidget.value());
        return *this;
    }

    RotatedBox& RotatedBox::quarterTurns(const std::size_t value)
    {
        if (value > 3u) {
            _quarterTurns = value % 4u;
            warning("RotatedBox", "Invalid quarterTurns value, expected < 4u. Normalizing to " + std::to_string(_quarterTurns) + "u.");
        } else {
            _quarterTurns = value;
        }
        return *this;
    }

    void RotatedBox::resolve(resolve_command& command)
    {
        // todo
        // command.rotate_child()
    }

}
}