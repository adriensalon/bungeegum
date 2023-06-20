#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Size.hpp>

namespace bungeegum {
namespace widgets {

    struct BoxConstraints {

        BoxConstraints(const float1 minWidth, const float1 maxWidth, const float1 minHeight, const float1 maxHeight);

        BoxConstraints(const float2 minSize, const float2 maxSize);

        BoxConstraints(const resolve_command& command);

        static BoxConstraints expand(const float1 width = infinity<float1>, const float1 height = infinity<float1>);

        static BoxConstraints loose(const Size size);

        static BoxConstraints tight(const Size size);

        // static BoxConstraints tightForWidth(const float1 width);

        // static BoxConstraints tightForHeight(const float1 height);

        // static BoxConstraints tightForFiniteWidth(const float1 width);

        // static BoxConstraints tightForFiniteHeight(const float1 height);

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

        float1 maxHeight() const;

        float1 maxWidth() const;

        Size smallest() const;

        //

        Size constrain(const Size size) const;

        Size constrainDimensions(const float1 width, const float1 height) const;

        float1 constrainHeight(const float1 height = infinity<float1>) const;

        Size constrainSizeAndAttemptToPreserveAspectRatio(const Size size) const;

        float1 constrainWidth(const float1 width = infinity<float1>) const;

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