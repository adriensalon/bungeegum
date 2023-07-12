#include <bungeegum_widgets/widgets/Container.hpp>

namespace bungeegum {
namespace widgets {

    Container& Container::alignment(const Alignment& value)
    {
        _alignment = value;
        return *this;
    }

    Container& Container::color(const Color value)
    {
        _color = value;
        return *this;
    }

    Container& Container::constraints(const BoxConstraints& value)
    {
        _constraints = value;
        return *this;
    }

    Container& Container::decoration(const BoxDecoration& value)
    {
        _decoration = value;
        return *this;
    }

    Container& Container::decoration(const ShapeDecoration& value)
    {
        _decoration = value;
        return *this;
    }

    Container& Container::foregroundDecoration(const BoxDecoration& value)
    {
        _foregroundDecoration = value;
        return *this;
    }

    Container& Container::foregroundDecoration(const ShapeDecoration& value)
    {
        _foregroundDecoration = value;
        return *this;
    }

    Container& Container::height(const float1 value)
    {
        if (!_size.has_value())
            _size = float2 { 0.f, 0.f };
        _size.value().y = value;
        return *this;
    }

    Container& Container::margin(const EdgeInsets& value)
    {
        _margin = value;
        return *this;
    }

    Container& Container::padding(const EdgeInsets& value)
    {
        _padding = value;
        return *this;
    }

    Container& Container::transform(const float4x4 value)
    {
        _transform = value;
        return *this;
    }

    Container& Container::transformAlignment(const Alignment& value)
    {
        _transformAlignment = value;
        return *this;
    }

    Container& Container::width(const float1 value)
    {
        if (!_size.has_value())
            _size = float2 { 0.f, 0.f };
        _size.value().x = value;
        return *this;
    }

    void Container::resolve(resolve_command& command)
    {
        // float2 _size;
        bool _has_child = (_adoptedChild.has_value());
        bool _has_size = (_size.has_value());

        if (_has_child) {
            command.resolve_child(_adoptedChild.value(), float2 { 0.f, 0.f }, float2 { 100.f, 100.f });
            command.position_child(_adoptedChild.value(), float2 { 0.f, 0.f });
        }
        if (_has_size)
            command.resize(_size.value());

        // todo
        // command.position_child()
    }

    void Container::draw(draw_command& command)
    {
        float2 _min_point = { 20.f, 20.f };
        float2 _max_point = { 600.f, 8.f };
        command.draw_rect_filled(_min_point, _max_point, _color, 12);
    }

}
}