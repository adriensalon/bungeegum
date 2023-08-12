#include <bungeegum_widgets/core/Alignment.hpp>

namespace bungeegum {
namespace widgets {

    Alignment Alignment::bottomCenter()
    {
        return Alignment(0.f, 1.f);
    }

    Alignment Alignment::bottomLeft()
    {
        return Alignment(-1.f, 1.f);
    }

    Alignment Alignment::bottomRight()
    {
        return Alignment(1.f, 1.f);
    }

    Alignment Alignment::center()
    {
        return Alignment(0.f, 0.f);
    }

    Alignment Alignment::centerLeft()
    {
        return Alignment(-1.f, 0.f);
    }

    Alignment Alignment::centerRight()
    {
        return Alignment(1.f, 0.f);
    }

    Alignment Alignment::topCenter()
    {
        return Alignment(0.f, -1.f);
    }

    Alignment Alignment::topLeft()
    {
        return Alignment(-1.f, -1.f);
    }

    Alignment Alignment::topRight()
    {
        return Alignment(1.f, -1.f);
    }

    Alignment::operator float2() const
    {
        return _value;
    }
}
}