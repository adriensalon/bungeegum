#include <bungeegum_widgets/core/FlexFit.hpp>
#include <bungeegum_widgets/widgets/Flex.hpp>


namespace bungeegum {
namespace widgets {

    Flex& Flex::crossAxisAlignment(const CrossAxisAlignment value)
    {
        _crossAxisAlignment = value;
        return *this;
    }

    Flex& Flex::direction(const Axis value)
    {
        _direction = value;
        return *this;
    }

    Flex& Flex::mainAxisAlignment(const MainAxisAlignment value)
    {
        _mainAxisAlignment = value;
        return *this;
    }

    Flex& Flex::mainAxisSize(const MainAxisSize value)
    {
        _mainAxisSize = value;
        return *this;
    }

    Flex& Flex::verticalDirection(const VerticalDirection value)
    {
        _verticalDirection = value;
        return *this;
    }

    void Flex::resolve(resolve_command& command)
    {
        struct childData {
            std::optional<float1> flexFactor;
            std::optional<FlexFit> flexFit;
            float2 resolvedSize;
            // float1 minMainAxisSpace;
            float1 maxMainAxisSpace;
        };

        std::vector<childData> _childrenData(_children.size());

        // 1. Layout each child with a null or zero flex factor (e.g., those that are not Expanded)
        // with unbounded main axis constraints and the incoming cross axis constraints. If the
        // crossAxisAlignment is CrossAxisAlignment.stretch, instead use tight cross axis constraints
        // that match the incoming max extent in the cross axis.

        float1 _mainAxisResolvedNoFlex = 0.f;
        float1 _maxCrossAxisResolvedNoFlex = 0.f;
        float2 _minSizeNoFlex = command.min_size();
        float2 _maxSizeNoFlex = command.max_size();
        if (_direction == Axis::horizontal) { // unbounded main axis
            // _minSizeNoFlex.x = zero<float1>;
            _maxSizeNoFlex.x = infinity<float1>;
        } else {
            // _minSizeNoFlex.y = zero<float1>;
            _maxSizeNoFlex.y = infinity<float1>;
        }
        if (_crossAxisAlignment == CrossAxisAlignment::stretch) { // tight cross axis if stretch
            if (_direction == Axis::horizontal) {
                _minSizeNoFlex.y = _maxSizeNoFlex.y;
            } else {
                _minSizeNoFlex.x = _maxSizeNoFlex.x;
            }
        }
        for (int1 _k = 0; _k < _children.size(); _k++) {
            childData& _childData = _childrenData[_k];
            resolve_command& _childCommand = get_resolve_command(_children[_k]);
            (void)_childCommand;
            // _childData.flexFactor = _childCommand.properties<float1>("flexFactor");
            // _childData.flexFit = _childCommand.properties<FlexFit>("flexFit");
            if (!_childData.flexFactor.has_value()) {
                runtime_widget& _child = _children[_k];
                _childData.resolvedSize = command.resolve_child(_child, _minSizeNoFlex, _maxSizeNoFlex);
                if (_direction == Axis::horizontal) {
                    _mainAxisResolvedNoFlex += _childData.resolvedSize.x;
                    _maxCrossAxisResolvedNoFlex = glm::max(_maxCrossAxisResolvedNoFlex, _childData.resolvedSize.y);
                } else {
                    _mainAxisResolvedNoFlex += _childData.resolvedSize.y;
                    _maxCrossAxisResolvedNoFlex = glm::max(_maxCrossAxisResolvedNoFlex, _childData.resolvedSize.x);
                }
            }
        }

        // 2. Divide the remaining main axis space among the children with non-zero flex factors
        // (e.g., those that are Expanded) according to their flex factor. For example, a child with
        // a flex factor of 2.f will receive twice the amount of main axis space as a child with a
        // flex factor of 1.f.

        float1 _sumFactors = zero<float1>;
        uint1 _countFactors = zero<uint1>;
        for (int _k = 0; _k < _children.size(); _k++) {
            childData& _childData = _childrenData[_k];
            if (_childData.flexFactor.has_value()) {
                _sumFactors += _childData.flexFactor.value();
                _countFactors++;
            }
        }
        for (int _k = 0; _k < _children.size(); _k++) {
            childData& _childData = _childrenData[_k];
            if (_childData.flexFactor.has_value()) {
                float1 _factorFrac = _childData.flexFactor.value() / _sumFactors;
                if (_direction == Axis::horizontal) {
                    // _childData.minMainAxisSpace = _factorFrac * (command.min_size().x - _mainAxisResolvedNoFlex);
                    _childData.maxMainAxisSpace = _factorFrac * (command.max_size().x - _mainAxisResolvedNoFlex);
                } else {
                    // _childData.minMainAxisSpace = _factorFrac * command.min_size().y;
                    _childData.maxMainAxisSpace = _factorFrac * (command.max_size().y - _mainAxisResolvedNoFlex);
                }
            }
        }

        // 3. Layout each of the remaining children with the same cross axis constraints as in step 1,
        // but instead of using unbounded main axis constraints, use max axis constraints based on
        // the amount of space allocated in step 2. Children with Flexible.fit properties that are
        // FlexFit.tight are given tight constraints (i.e., forced to fill the allocated space), and
        // children with Flexible.fit properties that are FlexFit.loose are given loose constraints
        // (i.e., not forced to fill the allocated space).

        float1 _mainAxisResolvedFlex = 0.f;
        float1 _maxCrossAxisResolvedFlex = 0.f;
        for (int1 _k = 0; _k < _children.size(); _k++) {
            childData& _childData = _childrenData[_k];
            if (_childData.flexFactor.has_value()) {
                runtime_widget& _child = _children[_k];

                float2 _minSizeFlexChild = command.min_size();
                float2 _maxSizeFlexChild = command.max_size();

                if (_direction == Axis::horizontal) { // max axis constraints based on the amount of space allocated in step 2.
                    _maxSizeFlexChild.x = _childData.maxMainAxisSpace;
                } else {
                    _maxSizeFlexChild.y = _childData.maxMainAxisSpace;
                }
                if (_crossAxisAlignment == CrossAxisAlignment::stretch) { // tight cross axis if stretch
                    if (_direction == Axis::horizontal) {
                        _minSizeFlexChild.y = _maxSizeFlexChild.y;
                    } else {
                        _minSizeFlexChild.x = _maxSizeFlexChild.x;
                    }
                }

                _childData.resolvedSize = command.resolve_child(_child, _minSizeFlexChild, _maxSizeFlexChild);

                if (_direction == Axis::horizontal) {
                    _mainAxisResolvedFlex += _childData.resolvedSize.x;
                    _maxCrossAxisResolvedFlex = glm::max(_maxCrossAxisResolvedFlex, _childData.resolvedSize.y);
                } else {
                    _mainAxisResolvedFlex += _childData.resolvedSize.y;
                    _maxCrossAxisResolvedFlex = glm::max(_maxCrossAxisResolvedFlex, _childData.resolvedSize.x);
                }
            }
        }

        // 4. The cross axis extent of the Flex is the maximum cross axis extent of the children (which
        // will always satisfy the incoming constraints).

        float1 _flexCrossAxisExtent = glm::max(_maxCrossAxisResolvedNoFlex, _maxCrossAxisResolvedFlex);

        // 5. The main axis extent of the Flex is determined by the mainAxisSize property. If the mainAxisSize
        // property is MainAxisSize.max, then the main axis extent of the Flex is the max extent of the
        // incoming main axis constraints. If the mainAxisSize property is MainAxisSize.min, then the main
        // axis extent of the Flex is the sum of the main axis extents of the children (subject to the
        // incoming constraints).

        float1 _flexMainAxisExtent;
        if (_mainAxisSize == MainAxisSize::max) {
            if (_direction == Axis::horizontal) {
                _flexMainAxisExtent = command.max_size().x;
            } else {
                _flexMainAxisExtent = command.max_size().y;
            }
        } else {
            _flexMainAxisExtent = _mainAxisResolvedNoFlex + _mainAxisResolvedFlex;
        }

        if (_direction == Axis::horizontal) {
            command.resize(float2 { _flexMainAxisExtent, _flexCrossAxisExtent });
        } else {
            command.resize(float2 { _flexCrossAxisExtent, _flexMainAxisExtent });
        }

        // 6. Determine the position for each child according to the mainAxisAlignment and the
        // crossAxisAlignment. For example, if the mainAxisAlignment is MainAxisAlignment.spaceBetween,
        // any main axis space that has not been allocated to children is divided evenly and placed
        // between the children.
    }
}
}