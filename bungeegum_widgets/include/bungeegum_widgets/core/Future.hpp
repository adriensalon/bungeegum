#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

    template <typename value_t>
    struct FutureOr;

    /// @brief The result of an asynchronous computation.
    template <typename value_t>
    struct Future {

        Future(std::future<value_t>&& future)
            : _value(std::move(future))
            , _isShared(false)
        {
        }

        Future(const std::shared_future<value_t>& future)
            : _value(future)
            , _isShared(true)
        {
        }

        template <typename thenValue_t>
        Future<thenValue_t> then(const std::function<FutureOr<thenValue_t>(const value_t&)>& onValue)
        {
            std::future<thenValue_t> _thenFutureValue = std::async([this, onValue]() {
                value_t _futureResult = std::get<std::future<value_t>>(_value).get();
                return static_cast<thenValue_t>(onValue(_futureResult));
            });
            Future<thenValue_t> _thenFuture(std::move(_thenFutureValue));
            return _thenFuture;
        }

        template <typename errorValue_t>
        Future<value_t> catchError(const std::function<void(const errorValue_t&)>& onError)
        {
        }

        Future<value_t> timeout(const std::chrono::milliseconds, const std::function<FutureOr<value_t>()>& onTimeout)
        {
        }

    private:
        std::variant<std::future<value_t>, std::shared_future<value_t>> _value;
        bool1 _isShared = false;
    };

    /// @brief
    template <typename value_t>
    struct FutureOr {

        FutureOr(const value_t)
        {
        }

        FutureOr(const Future<value_t>&)
        {
        }

        operator value_t() const
        {
            if (_isFuture) {

            } else {
                return std::get<value_t>(_value);
            }
        }

    private:
        std::variant<value_t, Future<value_t>> _value;
        bool1 _isFuture = false;
    };

}
}