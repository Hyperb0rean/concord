#pragma once

#include "concord/future/concepts.hpp"
#include "concord/future/future_local.hpp"

namespace concord::future::thunk {

template<typename V>
struct [[nodiscard]] Ready {
    using ValueType = V;
    using State = Unit;

    [[no_unique_address]] ValueType value;

    explicit Ready(V v) : value(std::move(v)) {}

    // Non-copyable
    Ready(const Ready&) = delete;
    auto operator=(const Ready&) -> Ready& = delete;

    Ready(Ready&&) = default;
    auto operator=(Ready&&) -> Ready& = default;

    template<Continuation<ValueType, Unit> Consumer>
    struct Computation {
        [[no_unique_address]] Consumer consumer;
        [[no_unique_address]] ValueType value;

        void call() {
            consumer.resume(std::move(value), Unit {});
        }
    };

    template<Continuation<ValueType, Unit> Consumer>
    auto materialize(Consumer&& c) -> future::Computation auto {
        return Computation<Consumer> {
            std::forward<Consumer>(c),
            std::move(value)
        };
    }
};

} // namespace concord::future::thunk