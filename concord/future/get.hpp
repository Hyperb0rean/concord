#pragma once

#include "axis/core/incel.hpp"
#include "concord/future/concepts.hpp"
#include "concord/os/sync/event.hpp"

namespace concord::future {

template<Thunk Future>
struct [[nodiscard]] GetTerminator {
    // NOLINTNEXTLINE(readability-identifier-naming)
    using value_type = value_of<Future>;
    using State = state_of<Future>;

    // Non-copyable
    GetTerminator(const GetTerminator&) = delete;
    auto operator=(const GetTerminator&) -> GetTerminator& = delete;

    GetTerminator(GetTerminator&&) = default;
    auto operator=(GetTerminator&&) -> GetTerminator& = default;

    struct GetDemand {
        GetTerminator* self;

        auto resume(value_type val, State) -> void {
            self->value.emplace(std::move(val));
            self->event.fire();
        }
    };

    using Computation = Materialize<Future, GetDemand>;

    explicit GetTerminator(Future&& f) :
        comp(f.materialize(GetDemand {this})) {}

    auto get() && noexcept(std::is_nothrow_move_constructible_v<value_type>)
        -> value_type {
        comp.call();
        event.wait();
        return std::move(value).get();
    }

    // TODO: Actually may not use Event in case synchronization is not neaded, should make static dispath
    os::sync::Event event;
    axis::InitializationCell<value_type> value;
    Computation comp;
};

template<Thunk Future> //NOLINTNEXTLINE(readability-identifier-naming)
auto Get(Future f) -> value_of<Future> {
    GetTerminator<Future> get {std::move(f)};
    return std::move(get).get();
}

} // namespace concord::future