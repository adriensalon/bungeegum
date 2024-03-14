#include <sstream>

#include <bungeegum_widgets/core/Color.hpp>

namespace bungeegum {
namespace widgets {

    namespace {

        // template <typename value_t>
        // bool is_length_equal(const value_t hex, const unsigned int length)
        // {
        //     std::stringstream _sstream_hex;
        //     _sstream_hex << std::hex << hex;
        //     std::string _str_hex = _sstream_hex.str();
        //     return (_str_hex.length() == length);
        // }

        float _linearizeColorComponent(const unsigned int colorComponent)
        {
            (void)colorComponent;
            // TODO
            return 1.f;
        }

        unsigned int _moduloComponent(const unsigned int value, const unsigned int operand)
        {
            unsigned int _value = value;
            while (_value > operand)
                _value -= operand;
            return _value;
        }

        unsigned int _integerDivision(const unsigned int a, const unsigned int b)
        {
            return static_cast<unsigned int>(glm::trunc(static_cast<float>(a) / b));
        }
    }

    Color::Color(const unsigned int hex)
    {
        uint32_t _hex = static_cast<uint32_t>(hex);
        _value = _hex;
    }

    Color Color::fromARGB(const unsigned int a, const unsigned int r, const unsigned int g, const unsigned int b)
    {
        uint8_t _a = static_cast<uint8_t>(_moduloComponent(a, 0xff));
        uint8_t _r = static_cast<uint8_t>(_moduloComponent(r, 0xff));
        uint8_t _g = static_cast<uint8_t>(_moduloComponent(g, 0xff));
        uint8_t _b = static_cast<uint8_t>(_moduloComponent(b, 0xff));
        uint32_t _hex = (((_a & 0xff) << 24) | ((_r & 0xff) << 16) | ((_g & 0xff) << 8) | ((_b & 0xff) << 0)) & 0xFFFFFFFF;
        return Color(_hex);
    }

    Color Color::fromRGBO(const unsigned int r, const unsigned int g, const unsigned int b, const float opacity)
    {
        uint8_t _r = static_cast<uint8_t>(_moduloComponent(r, 0xff));
        uint8_t _g = static_cast<uint8_t>(_moduloComponent(g, 0xff));
        uint8_t _b = static_cast<uint8_t>(_moduloComponent(b, 0xff));
        uint32_t _hex = (((static_cast<uint8_t>(glm::trunc((opacity * 0xff))) & 0xff) << 24) | ((_r & 0xff) << 16) | ((_g & 0xff) << 8) | ((_b & 0xff) << 0)) & 0xFFFFFFFF;
        return Color(_hex);
    }

    Color Color::alphaBlend(const Color foreground, const Color background)
    {
        uint8_t alpha = static_cast<uint8_t>(foreground.alpha());
        if (alpha == 0x00) { // Foreground completely transparent.
            return background;
        }
        uint8_t invAlpha = 0xff - alpha;
        uint8_t backAlpha = static_cast<uint8_t>(background.alpha());
        if (backAlpha == 0xff) { // Opaque background case
            return fromARGB(
                0xff,
                _integerDivision(alpha * foreground.red() + invAlpha * background.red(), 0xff),
                _integerDivision(alpha * foreground.green() + invAlpha * background.green(), 0xff),
                _integerDivision(alpha * foreground.blue() + invAlpha * background.blue(), 0xff));
        } else { // General case
            backAlpha = static_cast<uint8_t>(_integerDivision(backAlpha * invAlpha, 0xff));
            int outAlpha = alpha + backAlpha;
            if (outAlpha == 0x00)
                log_error("Color", "Error askip");
            return fromARGB(
                outAlpha,
                _integerDivision(foreground.red() * alpha + background.red() * backAlpha, outAlpha),
                _integerDivision(foreground.green() * alpha + background.green() * backAlpha, outAlpha),
                _integerDivision(foreground.blue() * alpha + background.blue() * backAlpha, outAlpha));
        }
    }

    unsigned int Color::getAlphaFromOpacity(float opacity)
    {
        return static_cast<uint8_t>(glm::round(glm::clamp(opacity, 0.f, 1.f) * 255));
    }

    unsigned int Color::alpha() const
    {
        return (0xff000000 & _value) >> 24u;
    }

    unsigned int Color::blue() const
    {
        return (0x000000ff & _value) >> 0u;
    }

    unsigned int Color::green() const
    {
        return (0x0000ff00 & _value) >> 8;
    }

    float Color::opacity() const
    {
        return static_cast<float_t>(alpha() / 0xFF);
    }

    unsigned int Color::red() const
    {
        return (0x00ff0000 & _value) >> 16;
    }

    float Color::computeLuminance() const
    {
        // See <https://www.w3.org/TR/WCAG20/#relativeluminancedef>
        float _r = _linearizeColorComponent(red() / 0xFF);
        float _g = _linearizeColorComponent(green() / 0xFF);
        float _b = _linearizeColorComponent(blue() / 0xFF);
        return 0.2126f * _r + 0.7152f * _g + 0.0722f * _b;
    }

    Color Color::withAlpha(const unsigned int a)
    {
        return fromARGB(a, red(), green(), blue());
    }

    Color Color::withBlue(const unsigned int b)
    {
        return fromARGB(alpha(), red(), green(), b);
    }

    Color Color::withGreen(const unsigned int g)
    {
        return fromARGB(alpha(), red(), g, blue());
    }

    Color Color::withOpacity(const float opacity)
    {
        if (opacity < 0.f)
            log_error("Color", "Invalid opacity < 0.f");
        if (opacity > 1.f)
            log_error("Color", "Invalid opacity > 1.f");
        return withAlpha(static_cast<uint32_t>(glm::round(255.f * opacity)));
    }

    Color Color::withRed(const unsigned int r)
    {
        return fromARGB(alpha(), r, green(), blue());
    }

    Color::operator unsigned int() const
    {
        return _value;
    }

    Color::operator float4() const
    {
        // https://stackoverflow.com/questions/3723846/convert-from-hex-color-to-rgb-struct-in-c
        return float4 {
            /* R */ ((_value >> 16) & 0xFF) / 255.f,
            /* G */ ((_value >> 8) & 0xFF) / 255.f,
            /* B */ ((_value)&0xFF) / 255.f,
            /* A */ ((_value >> 24) & 0xFF) / 255.f
        };
    }

    bool Color::operator==(const Color& other)
    {
        return (_value == other._value);
    }

}
}