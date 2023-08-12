#include <bungeegum_widgets/widgets/AspectRatio.hpp>

namespace bungeegum {
namespace widgets {

    AspectRatio& AspectRatio::aspectRatio(const float1 value)
    {
        _aspectRatio = value;
        return *this;
    }

    AspectRatio& AspectRatio::child(const runtime_widget& value)
    {
        if (_childWidget.has_value())
            abandon(this, _childWidget.value());
        _childWidget = value;
        adopt(this, _childWidget.value());
        return *this;
    }

    void AspectRatio::resolve(resolve_command& command)
    {
        if (_childWidget.has_value()) {
            if (_aspectRatio.has_value()) {

                //
                //
                //

            } else {
                command.resize(command.max_size());
            }
        } else {
            command.resize(command.max_size());
        }
    }

}
}