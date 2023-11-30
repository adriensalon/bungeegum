#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief An immutable 32 bit color value in ARGB format.
    /// @details Consider the light teal of the Flutter logo. It is fully opaque, with a red
    /// channel value of 0x42 (66), a green channel value of 0xA5 (165), and a blue channel value
    /// of 0xF5 (245). In the common "hash syntax" for color values, it would be described as
    /// #42A5F5.
    struct Color {
        Color() = default;
        Color(const Color& other) = default;
        Color& operator=(const Color& other) = default;
        Color(Color&& other) = default;
        Color& operator=(Color&& other) = default;

        /// @brief Construct a color from the lower 32 bits of an int.
        /// @details The bits are interpreted as follows:
        /// - Bits 24-31 are the alpha value.
        /// - Bits 16-23 are the red value.
        /// - Bits 8-15 are the green value.
        /// - Bits 0-7 are the blue value.
        /// In other words, if AA is the alpha value in hex, RR the red value in hex, GG the green
        /// value in hex, and BB the blue value in hex, a color can be expressed as
        /// Color(0xAARRGGBB). For example, to get a fully opaque orange, you would use
        /// Color(0xFFFF9000) (FF for the alpha, FF for the red, 90 for the green, and 00 for the
        /// blue).
        Color(const unsigned int hex);

        /// @brief Construct a color from 8 bits integers.
        /// @param a is the alpha value, with 0 being transparent and 255 being fully opaque.
        /// @param r is red, from 0 to 255.
        /// @param g is green, from 0 to 255.
        /// @param b is green, from 0 to 255.
        /// @details Out of range values are brought into range using modulo 255.
        /// See also fromRGBO, which takes the alpha value as a floating point value.
        static Color fromARGB(const unsigned int a, const unsigned int r, const unsigned int g, const unsigned int b);

        /// @brief Create a color from red, green, blue, and opacity, similar to rgba() in CSS.
        /// @param r is red, from 0 to 255.
        /// @param g is green, from 0 to 255.
        /// @param b is green, from 0 to 255.
        /// @param opacity is alpha channel of this color, with 0.f being transparent and 1.f being
        /// fully opaque.
        /// @details Out of range values are brought into range using modulo 255.
        /// See also fromARGB, which takes the opacity as an integer value.
        static Color fromRGBO(const unsigned int r, const unsigned int g, const unsigned int b, const float opacity);

        /// @brief Combine the foreground color as a transparent color over top of a background
        /// color, and return the resulting combined color.
        /// @details This uses standard alpha blending ("SRC over DST") rules to produce a blended
        /// color from two colors. This can be used as a performance enhancement when trying to
        /// avoid needless alpha blending compositing operations for two things that are solid
        /// colors with the same shape, but overlay each other: instead, just paint one with the
        /// combined color.
        static Color alphaBlend(const Color foreground, const Color background);

        /// @brief Returns an alpha value representative of the provided opacity value.
        static unsigned int getAlphaFromOpacity(float opacity);

        /// @brief The alpha channel of this color in an 8 bit value.
        /// @details A value of 0 means this color is fully transparent. A value of 255 means this
        /// color is fully opaque.
        unsigned int alpha() const;

        /// @brief The blue channel of this color in an 8 bit value.
        unsigned int blue() const;

        /// @brief The green channel of this color in an 8 bit value.
        unsigned int green() const;

        /// @brief The alpha channel of this color as a float.
        /// @details A value of 0.f means this color is fully transparent. A value of 1.f means
        /// this color is fully opaque.
        float opacity() const;

        /// @brief The red channel of this color in an 8 bit value.
        unsigned int red() const;

        /// @brief Returns a brightness value between 0.f for darkest and 1.f for lightest.
        /// @details Represents the relative luminance of the color. This value is computationally
        /// expensive to calculate. See https://en.wikipedia.org/wiki/Relative_luminance.
        float computeLuminance() const;

        /// @brief Returns a new color that matches this color with the alpha channel replaced with
        /// a (which ranges from 0 to 255).
        Color withAlpha(const unsigned int a);

        /// @brief Returns a new color that matches this color with the blue channel replaced with
        /// b (which ranges from 0 to 255).
        Color withBlue(const unsigned int b);

        /// @brief Returns a new color that matches this color with the green channel replaced with
        /// g (which ranges from 0 to 255).
        Color withGreen(const unsigned int g);

        /// @brief Returns a new color that matches this color with the alpha channel replaced with
        /// the given opacity (which ranges from 0.0 to 1.0).
        Color withOpacity(const float opacity);

        /// @brief Returns a new color that matches this color with the alpha channel replaced with
        /// the given opacity (which ranges from 0.0 to 1.0).
        Color withRed(const unsigned int r);

        /// @brief A 32 bit value representing this color.
        operator unsigned int() const;

        /// @brief A float4 RGBA value representing this color.
        operator float4() const;

        /// @brief The equality operator
        bool operator==(const Color& other);

    private:
        template <typename archive_t>
        void load(archive_t& archive)
        {
            int _red, _green, _blue, _alpha;
            archive(_red);
            archive(_green);
            archive(_blue);
            archive(_alpha);
            *this = fromARGB(
                math::clamp<unsigned int>(_alpha, 0u, 255u),
                math::clamp<unsigned int>(_red, 0u, 255u),
                math::clamp<unsigned int>(_green, 0u, 255u),
                math::clamp<unsigned int>(_blue, 0u, 255u));
        }

        template <typename archive_t>
        void save(archive_t& archive) const
        {
            archive(cereal::make_nvp("red", math::clamp<unsigned int>(red(), 0u, 255u)));
            archive(cereal::make_nvp("green", math::clamp<unsigned int>(green(), 0u, 255u)));
            archive(cereal::make_nvp("blue", math::clamp<unsigned int>(blue(), 0u, 255u)));
            archive(cereal::make_nvp("alpha", math::clamp<unsigned int>(alpha(), 0u, 255u)));
        }

        uint32_t _value = zero<uint32_t>;
        friend class cereal::access;
    };
}

// largest ?
}