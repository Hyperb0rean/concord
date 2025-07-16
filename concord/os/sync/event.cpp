
#include "event.hpp"

#include <cassert>

#include "concord/os/wait/wait.hpp"

namespace concord::os::sync {

auto Event::wait() noexcept -> void {
    if (auto state = _state.load(std::memory_order::acquire);
        state != State::Fired) {
        os::wait(_state, state, std::memory_order::acquire);
    }
}

auto Event::fire() noexcept -> void {
    auto&& token = prepare_wake(_state);
    _state.store(State::Fired, std::memory_order::release);
    wake_all(std::move(token));
}

} // namespace concord::os::sync