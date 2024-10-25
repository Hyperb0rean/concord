#pragma once

// Mostly copy of https://gitlab.com/Lipovsky/futex_like

#include <atomic>
#include <cstdint>

#include "concord/syscall/wait/atomic/u32.hpp"
#include "concord/syscall/wait/atomic/u64.hpp"

namespace concord::syscall {

using AtomicRefUint64Low =
    atomic::AtomicRefUint64<atomic::AtomicHalfUint64::Low>;
using AtomicRefUint64High =
    atomic::AtomicRefUint64<atomic::AtomicHalfUint64::High>;

inline auto low_half(std::atomic<uint64_t>& atomic) {
    return AtomicRefUint64Low {atomic};
}

inline auto high_half(std::atomic<uint64_t>& atomic) {
    return AtomicRefUint64High {atomic};
}

auto wait(
    std::atomic<uint32_t>& atomic, //
    uint32_t old, //
    std::memory_order mo = std::memory_order::seq_cst
) -> uint32_t;

auto wait(
    AtomicRefUint64High atomic, //
    uint32_t old, //
    std::memory_order mo = std::memory_order::seq_cst
) -> uint64_t;

auto wait(
    AtomicRefUint64Low atomic, //
    uint32_t old, //
    std::memory_order mo = std::memory_order::seq_cst
) -> uint64_t;

// Unimplemented
auto wait_timed(
    std::atomic<uint32_t>& atomic, //
    uint32_t old, //
    std::chrono::milliseconds timeout, //
    std::memory_order mo = std::memory_order::seq_cst
) -> void;

struct [[nodiscard]] WakeToken {
    uint32_t* location;
};

inline auto prepare_wake(std::atomic<uint32_t>& atomic) -> WakeToken {
    return {atomic::AtomicRefUint32 {atomic}.location()};
}

template<atomic::AtomicHalfUint64 Half>
auto prepare_wake(atomic::AtomicRefUint64<Half> atomic_ref) -> WakeToken {
    return {atomic_ref.location()};
}

auto wake_one(WakeToken token) -> void;
auto wake_all(WakeToken token) -> void;

} // namespace concord::syscall