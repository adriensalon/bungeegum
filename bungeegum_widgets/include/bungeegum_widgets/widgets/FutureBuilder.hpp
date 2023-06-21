#pragma once

#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/core/Future.hpp>

namespace bungeegum {
namespace widgets {

    template <typename futureValue_t>
    struct FutureBuilder {

        template <typename childWidget_t>
        FutureBuilder& builder(const std::function<childWidget_t&(const futureValue_t&)>& value)
        {
            // reset tout si deja defini && deja invoke ?
            _builderFunction = [this, value](const futureValue_t& futureValue) {
                if (_child.has_value())
                    abandon(this, _child.value());
                _child = adopt(this, value(futureValue));
                must_resolve(this);
            };
            _event.on_trigger([this, value](const futureValue_t& futureValue) {
                _builderFunction.value()(futureValue);
                _futureDataBuildDone = true;
            });
            if (!_initialDataBuildDone) {
                if (_initialData.has_value()) {
                    _builderFunction.value()(_initialData.value());
                    _initialDataBuildDone = true;
                }
            }
            return *this;
        }

        FutureBuilder& future(std::future<futureValue_t>&& value)
        {
            // que faire si deja lance ?
            _event.trigger(std::move(value));
            return *this;
        }

        FutureBuilder& future(const std::shared_future<futureValue_t>& value)
        {
            // que faire si deja lance ?
            _event.trigger(value);
            return *this;
        }

        FutureBuilder& initialData(const futureValue_t& value)
        {
            // que faire si deja lance ?
            if (!_initialDataBuildDone) {
                if (_builderFunction.has_value()) {
                    _builderFunction.value()(value);
                    _initialDataBuildDone = true;
                } else {
                    _initialData = value;
                }
            }
            return *this;
        }

    private:
        friend struct access;
        void resolve(resolve_command& command)
        {
            if (_child.has_value()) {
                float2 _childSize = command.resolve_child(_child.value(), command.min_size(), command.max_size());
                command.position_child(_child.value(), zero<float2>);
                command.resize(_childSize);
            } else
                command.resize(command.max_size());
        }

        std::optional<adopted_widget> _child = std::nullopt;
        std::optional<std::function<void(const futureValue_t&)>> _builderFunction = std::nullopt;
        std::optional<futureValue_t> _initialData = std::nullopt;
        event<futureValue_t> _event = {};
        bool1 _initialDataBuildDone = false;
        bool1 _futureDataBuildDone = false;
    };

}
}