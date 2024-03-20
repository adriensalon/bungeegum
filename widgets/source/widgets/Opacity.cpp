#include <bungeegum_widgets/widgets/Opacity.hpp>

namespace bungeegum {
namespace widgets {

    Opacity& Opacity::child(const widget_id& value)
    {
        if (_childWidget.has_value())
            abandon(this, _childWidget.value());
        _childWidget = value;
        adopt(this, _childWidget.value());
        return *this;
    }

    Opacity& Opacity::opacity(const float value)
    {
        if (value > 1.f) {
            _opacity = 1.f;
            warning("Opacity", "Wrong opacity value, expected < 1.f. Clamping to 1.f.");
        } else if (value < 0.f) {
            _opacity = 0.f;
            warning("Opacity", "Wrong opacity value, expected > 0.f. Clamping to 0.f.");
        } else {
            _opacity = value;
        }
        return *this;
    }

    void Opacity::draw(draw_command& command)
    {
        if (!_childWidget.has_value() || (_opacity == zero<float>)) {
            return;
        }
        if (_opacity < 1.f) {
            command.use_opacity(_opacity);
        } 
        // command.draw_child(_childWidget.value());
        command.draw_children();
    }

}
}