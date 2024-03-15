#include <bungeegum_widgets/core/Offset.hpp>
#include <bungeegum_widgets/core/Size.hpp>
#include <bungeegum_widgets/widgets/ColoredBox.hpp>
#include <bungeegum_widgets/widgets/Padding.hpp>

namespace bungeegum {
namespace widgets {

    ColoredBox& ColoredBox::child(const widget_id& value)
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
        _color = Color(0xFF8888FF);

#if defined(__HOTRELOADING__)
        // std::cout << "oui" << std::endl;
        // message("from hotreload");
#endif
        // std::cout << bungeegum::detail::animations_manager::stat_test++ << std::endl;
        // std::cout << bungeegum::detail::global().animations.stat_test++ << std::endl;

        // auto gg = bungeegum::runtime_widget(this);
        // (void)gg;
        // message("has parent = " + std::to_string(bungeegum::has_parent(this)));
        // std::cout << "ok n";

        // message("hey this is a message");

        // auto& pp = bungeegum::make<Padding>();
        // must_resolve(pp);

        // message("is contextual = " + std::to_string(bungeegum::is_contextual<float>("myfloat")));

        // bungeegum::adopt(this, bungeegum::make<Padding>().padding(EdgeInsets::fromLTRB(0.f, 100.f, 0.f, 50.f)));
        // bungeegum::adopt(this, bungeegum::make<int>());

        if (_childWidget.has_value()) {
            float2 _childSize = command.resolve_child(_childWidget.value(), command.get_min_size(), command.get_max_size());
            command.position_child(_childWidget.value(), zero<float2>);
            command.resize(_childSize);
        } else
            command.resize(command.get_max_size());
			

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

    void ColoredBox::draw(draw_command& command)
    {
        _color = Color(0xFF8888FF);
        float2 _min_point = {10.f, 100.f};
        float2 _max_point = {50.f, 500.f};
        command.draw_rect_filled(_min_point, _max_point, _color);

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // ff += 0.0001f;
        // std::cout << "color box  = " << ff << std::endl;
    }

}
}