#include "mutex.hpp"

#include <utility>

#include "concord/os/sync/spinner.hpp"
#include "concord/os/wait/wait.hpp"

namespace concord::os::sync {

auto Mutex::lock() -> void {
    Spinner spinner;
    uint32_t current = State::Free;

    while (_state.compare_exchange_weak(
        current,
        State::Locked,
        std::memory_order::acquire,
        std::memory_order::relaxed
    )) {
        if (spinner.keep_spinning()) {
            spinner();
            current = State::Free;
            continue;
        }

        while (_state.exchange(State::Awaited, std::memory_order::acquire)
               != State::Free) {
            wait(_state, State::Awaited, std::memory_order::relaxed);
        }
        return;
    }
}

auto Mutex::unlock() -> void {
    auto&& token = prepare_wake(_state);
    if (_state.exchange(State::Free, std::memory_order::release)
        == State::Awaited) {
        wake_one(std::move(token));
    }
}

} // namespace concord::os::sync