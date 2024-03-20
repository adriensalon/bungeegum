#include <bungeegum_widgets/widgets/ListView.hpp>
#include <bungeegum_widgets/core/BoxConstraints.hpp>

namespace bungeegum {
namespace widgets {

    ListView& ListView::children(const std::vector<widget_id>& value)
    {
        _childrenWidgets.clear();
        _childrenWidgets.insert(_childrenWidgets.begin(), value.begin(), value.end());
        for (const widget_id& _child : _childrenWidgets) {
            adopt(this, _child);
        }
        return *this;
    }
    
    void ListView::resolve(resolve_command& command)
    {        
        float2 _maxSize = command.get_max_size();
        Size _paddingSize = _padding.collapsedSize();
        Size _offsetSize = { _padding.left(), _padding.top() };
        Size _constrainedSize = Size { _maxSize } - _paddingSize;        
        for (const widget_id& _child : _childrenWidgets) {
            float2 _childSize = command.resolve_child(_child, _constrainedSize, _constrainedSize);
            command.position_child(_child, _offsetSize);
            if (_scrollDirection == Axis::vertical) {
                _offsetSize = _offsetSize + Size { 0.f, _childSize.y };
            } else {
                _offsetSize = _offsetSize + Size { _childSize.x, 0.f };
            }
        }        
        command.resize(_maxSize);
    }

    void ListView::draw(draw_command& command)
    {
        command.draw_children();
    }


}
}