#pragma once

#include <functional>
#include <utility>

#include "concord/future/concepts.hpp"

namespace concord::future::thunk {

template<Thunk Producer, typename Bind>
struct [[nodiscard]] With {
    [[no_unique_address]] Producer producer;
    [[no_unique_address]] Bind state_bind;

    using ValueType = value_of<Producer>;
    using InputState = state_of<Producer>;
    using State = std::invoke_result_t<Bind, InputState>;

    // Non-copyable
    With(const With&) = delete;
    auto operator=(const With&) -> With& = delete;

    With(With&&) = default;
    auto operator=(With&&) -> With& = default;

    explicit With(Producer&& p, Bind s) :
        producer(std::move(p)),
        state_bind(std::move(s)) {}

    template<Continuation<ValueType, State> Consumer>
    struct Demand {
        [[no_unique_address]] Consumer consumer;
        [[no_unique_address]] Bind state_bind;

        auto resume(ValueType v, InputState s) {
            consumer.resume(
                std::move(v),
                std::invoke(state_bind, std::move(s))
            );
        }
    };

    template<Continuation<ValueType, State> Consumer>
    auto materialize(Consumer&& c) -> Computation auto {
        return producer.materialize(
            Demand {std::move(c), std::move(state_bind)}
        );
    }
};

} // namespace concord::future::thunk