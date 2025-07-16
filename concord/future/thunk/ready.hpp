#pragma once

#include "concord/future/concepts.hpp"

namespace concord::future::thunk {

template<typename V>
struct [[nodiscard]] Ready {
    using value_type = V; // NOLINT(readability-identifier-naming)

    value_type value;

    explicit Ready(V v) : value(std::move(v)) {}

    // Non-copyable
    Ready(const Ready&) = delete;
    auto operator=(const Ready&) -> Ready& = delete;

    Ready(Ready&&) = default;
    auto operator=(Ready&&) -> Ready& = default;

    template<Continuation<V> Consumer>
    struct ReadyComputation {
        Consumer consumer;
        V value;

        void call() {
            consumer.resume(std::move(value));
        }
    };

    template<Continuation<V> Consumer>
    auto materialize(Consumer&& c) -> Computation auto {
        return ReadyComputation<Consumer> {
            std::forward<Consumer>(c),
            std::move(value)
        };
    }
};

} // namespace concord::future::thunk