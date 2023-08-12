#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>
#include <bungeegum_widgets/widgets/ColoredBox.hpp>
#include <bungeegum_widgets/widgets/Padding.hpp>

namespace bungeegum {
namespace widgets {

    ColoredBox& ColoredBox::child(const runtime_widget& value)
    {
        if (_childWidget.has_value())
            abandon(this, _childWidget.value());
        _childWidget = value;
        adopt(this, _childWidget.value());
        return *this;
    }

    ColoredBox& ColoredBox::color(const Color value)
    {
        _color = value;
        return *this;
    }

    void ColoredBox::resolve(resolve_command& command)
    {
        // _color = Color(0xFF8888FF);

#if defined(__HOTRELOADING__)
        // std::cout << "oui" << std::endl;
#endif
        // std::cout << bungeegum::detail::animations_manager::stat_test++ << std::endl;
        // std::cout << bungeegum::detail::global().animations.stat_test++ << std::endl;

        // auto gg = bungeegum::runtime_widget(this);
        // (void)gg;
        // log_message("has parent = " + std::to_string(bungeegum::has_parent(this)));
        // std::cout << "ok n";

        // log_message("hey this is a message");

        // auto& pp = bungeegum::make<Padding>();
        // must_resolve(pp);

        log_message("is contextual = " + std::to_string(bungeegum::is_contextual<float>("myfloat")));

        // bungeegum::adopt(this, bungeegum::make<Padding>().padding(EdgeInsets::fromLTRB(0.f, 100.f, 0.f, 50.f)));
        // bungeegum::adopt(this, bungeegum::make<int>());

        if (_childWidget.has_value()) {
            float2 _childSize = command.resolve_child(_childWidget.value(), command.min_size(), command.max_size());
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.max_size());

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

    void ColoredBox::draw(draw_command& command)
    {
        float2 _min_point = command.resolved_position();
        float2 _max_point = _min_point + command.resolved_size();
        command.draw_rect_filled(_min_point, _max_point, _color);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // ff += 0.0001f;
        // std::cout << "color box  = " << ff << std::endl;
    }

}
}