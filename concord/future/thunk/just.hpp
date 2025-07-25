#pragma once

#include "concord/future/concepts.hpp"

namespace concord::future::thunk {

struct [[nodiscard]] Just {
    using ValueType = Unit;
    using State = Unit;

    Just() = default;

    // Non-copyable
    Just(const Just&) = delete;
    auto operator=(const Just&) -> Just& = delete;

    Just(Just&&) = default;
    auto operator=(Just&&) -> Just& = default;

    template<Continuation<Unit, Unit> Consumer>
    struct Computation {
        [[no_unique_address]] Consumer consumer;

        void call() {
            consumer.resume(Unit {}, Unit {});
        }
    };

    template<Continuation<Unit, Unit> Consumer>
    auto materialize(Consumer&& c) -> future::Computation auto {
        return Computation<Consumer> {std::forward<Consumer>(c)};
    }
};

} // namespace concord::future::thunk