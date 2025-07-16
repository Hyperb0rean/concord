#pragma once

#include <utility>

#include "concord/future/concepts.hpp"

namespace concord::future::comb {

template<typename S>
struct With {
    using State = S;

    [[no_unique_address]] State state;

    // Non-copyable
    With(const With&) = delete;
    auto operator=(const With&) -> With& = delete;

    With(With&&) = default;
    auto operator=(With&&) -> With& = default;

    explicit With(State s) : state(std::move(s)) {}

    template<Thunk InputFuture>
    auto then(InputFuture) -> Future<value_of<InputFuture>, S> auto {}
};

} // namespace concord::future::comb