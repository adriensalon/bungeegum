#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    struct BoxConstraints {

        BoxConstraints(const float minWidth, const float maxWidth, const float minHeight, const float maxHeight);

        BoxConstraints(const float2 minSize, const float2 maxSize);

        BoxConstraints(const resolve_command& command);

        static BoxConstraints expand(const float width = infinity<float>, const float height = infinity<float>);

        static BoxConstraints loose(const Size size);

        static BoxConstraints tight(const Size size);

        // static BoxConstraints tightForWidth(const float width);

        // static BoxConstraints tightForHeight(const float height);

        // static BoxConstraints tightForFiniteWidth(const float width);

        // static BoxConstraints tightForFiniteHeight(const float height);

        Size biggest() const;

        BoxConstraints flipped() const;

        bool hasBoundedHeight() const;

        bool hasBoundedWidth() const;

        bool hasInfiniteHeight() const;

        bool hasInfiniteWidth() const;

        bool hasTightHeight() const;

        bool hasTightWidth() const;

        bool isNormalized() const;

        bool isTight() const;

        float maxHeight() const;

        float maxWidth() const;

        Size smallest() const;

        //

        Size constrain(const Size size) const;

        Size constrainDimensions(const float width, const float height) const;

        float constrainHeight(const float height = infinity<float>) const;

        Size constrainSizeAndAttemptToPreserveAspectRatio(const Size size) const;

        float constrainWidth(const float width = infinity<float>) const;

        BoxConstraints() = default;
        BoxConstraints(const BoxConstraints& other) = default;
        BoxConstraints& operator=(const BoxConstraints& other) = default;
        BoxConstraints(BoxConstraints&& other) = default;
        BoxConstraints& operator=(BoxConstraints&& other) = default;

    private:
        float2 _minSize = zero<float2>;
        float2 _maxSize = infinity<float2>;
    };

}
}