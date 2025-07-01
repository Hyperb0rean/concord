#include "wait_group.hpp"

#include "concord/os/wait/atomic/u64.hpp"
#include "concord/os/wait/wait.hpp"

namespace concord::os::sync {

auto WaitGroup::add(std::size_t count) -> void {
    _state.fetch_add(count << 32, std::memory_order::release);
}

auto WaitGroup::done() -> void {
    auto token = os::prepare_wake(
        os::atomic::AtomicRefUint64<atomic::AtomicHalfUint64::High> {_state}
    );
    uint64_t const state =
        _state.fetch_sub(1ul << 32, std::memory_order::release);
    uint32_t const work = state >> 32;
    uint32_t const waiters = static_cast<uint32_t>(state);
    if (work == 1 && waiters != 0) {
        os::wake_all(token);
    }
}

auto WaitGroup::wait() -> void {
    uint64_t state = _state.fetch_add(1, std::memory_order::acquire);
    while (state >> 32 != 0) {
        state = os::wait(
            atomic::AtomicRefUint64<atomic::AtomicHalfUint64::High>(_state),
            state >> 32,
            std::memory_order::acquire
        );
    }
    _state.store(0, std::memory_order::release);
}
} // namespace concord::os::sync