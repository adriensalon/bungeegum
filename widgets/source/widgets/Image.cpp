#include <bungeegum_widgets/widgets/Image.hpp>

namespace bungeegum {
namespace widgets {

    Image& Image::fromFile(const std::filesystem::path& value)
    {
        _texture = texture { value };
        return *this;
    }

    Image& Image::fromFutureTexture(std::future<texture>&& value)
    {
        _futureTexture.emplace(std::move(value));
        return *this;
    }

    Image& Image::fromMemory(const std::vector<unsigned char>& pixels, const float2 size)
    {
        _texture = texture(pixels, static_cast<uint2>(size));
        return *this;
    }

    Image& Image::fromTexture(texture&& value)
    {
        _texture = std::move(value);
        return *this;
    }

    Image& Image::alignment(const Alignment& value)
    {
        _alignment = value;
        return *this;
    }

    Image& Image::centerSlice(const std::optional<Rect>& value)
    {
        _centerSlice = value;
        return *this;
    }

    Image& Image::color(const std::optional<Color>& value)
    {
        _color = value;
        return *this;
    }



    
    void Image::resolve(resolve_command& command)
    {        
        // float2 _maxSize = command.get_max_size();
        // Size _paddingSize = _padding.collapsedSize();
        // Size _offsetSize = { _padding.left(), _padding.top() };
        // Size _constrainedSize = Size { _maxSize } - _paddingSize;        
        // for (const widget_id& _child : _childrenWidgets) {
        //     float2 _childSize = command.resolve_child(_child, _constrainedSize, _constrainedSize);
        //     command.position_child(_child, _offsetSize);
        //     if (_scrollDirection == Axis::vertical) {
        //         _offsetSize = _offsetSize + Size { 0.f, _childSize.y };
        //     } else {
        //         _offsetSize = _offsetSize + Size { _childSize.x, 0.f };
        //     }
        // }        
        // command.resize(_maxSize);
    }

    void Image::draw(draw_command& command)
    {
        command.draw_children();
    }


}
}