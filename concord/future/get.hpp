#pragma once

#include "axis/core/incel.hpp"
#include "concord/future/concepts.hpp"
#include "concord/os/sync/event.hpp"

namespace concord::future {

template<Thunk Future>
struct [[nodiscard]] GetTerminator {
    using ValueType = value_of<Future>;
    using State = state_of<Future>;

    // Non-copyable
    GetTerminator(const GetTerminator&) = delete;
    auto operator=(const GetTerminator&) -> GetTerminator& = delete;

    GetTerminator(GetTerminator&&) = default;
    auto operator=(GetTerminator&&) -> GetTerminator& = default;

    struct Demand {
        GetTerminator* self;

        auto resume(ValueType val, State) -> void {
            self->value.emplace(std::move(val));
            self->event.fire();
        }
    };

    using Computation = Materialize<Future, Demand>;

    explicit GetTerminator(Future&& f) : comp(f.materialize(Demand {this})) {}

    auto get() noexcept(std::is_nothrow_move_constructible_v<ValueType>)
        -> ValueType {
        comp.call();
        event.wait();
        return std::move(value).get();
    }

    // TODO: Actually may not use Event in case synchronization is not neaded, should make static dispath
    os::sync::Event event;
    axis::InitializationCell<ValueType> value;
    Computation comp;
};

template<Thunk Future> //NOLINTNEXTLINE(readability-identifier-naming)
auto Get(Future&& f) -> value_of<Future> {
    GetTerminator pin {std::move(f)};
    return pin.get();
}

} // namespace concord::future