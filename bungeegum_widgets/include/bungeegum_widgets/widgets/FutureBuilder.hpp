#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    /// @brief A widget that builds itself based on the result of a std::future or
    /// std::shared_future.
    template <typename futureValue_t>
    struct FutureBuilder {

        /// @brief The build strategy currently used.
        /// @details Initial data can be used to fire the build callback only once in the lifetime
        /// of this FutureBuilder, the first time initialData() and builder() are defined.
        FutureBuilder& builder(const std::function<runtime_widget(const futureValue_t&)>& value)
        {
            // reset tout si deja defini && deja invoke ?
            _builderFunction = [this, value](const futureValue_t& futureValue) {
                if (_childWidget.has_value())
                    abandon(this, _childWidget.value());
                _childWidget = value(futureValue);
                adopt(this, _childWidget.value());
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

        /// @brief The asynchronous computation to which this builder is currently connected.
        FutureBuilder& future(std::future<futureValue_t>&& value)
        {
            // que faire si deja lance ?
            _event.trigger(std::move(value));
            return *this;
        }

        /// @brief The asynchronous computation to which this builder is currently connected.
        FutureBuilder& future(const std::shared_future<futureValue_t>& value)
        {
            // que faire si deja lance ?
            _event.trigger(value);
            return *this;
        }

        /// @brief The data that will be used to build until the future has completed.
        /// @details Initial data can be used to fire the build callback only once in the lifetime
        /// of this FutureBuilder, the first time initialData() and builder() are defined.
        FutureBuilder& initialData(const futureValue_t& value)
        {
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
            if (_childWidget.has_value()) {
                float2 _childSize = command.resolve_child(_childWidget.value(), command.min_size(), command.max_size());
                command.position_child(_childWidget.value(), zero<float2>);
                command.resize(_childSize);
            } else {
                command.resize(command.max_size());
            }
        }

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<std::function<void(const futureValue_t&)>> _builderFunction = std::nullopt;
        std::optional<futureValue_t> _initialData = std::nullopt;
        event<futureValue_t> _event = {};
        bool1 _initialDataBuildDone = false;
        bool1 _futureDataBuildDone = false;
    };

}
}