#pragma once
#include "synchapi.h"
#include "wait.hpp"

namespace concord::syscall::detail {

auto wait_timed(uint32_t* addr, uint32_t expected, uint32_t millis) -> int {
    return WaitOnAddress(addr, expected, 4, millis);
}

auto wait(uint32_t* addr, uint32_t old) -> int {
    return wait_timed(addr, old, /*millis=*/0);
}

auto wake(uint32_t* addr, size_t count) -> int {
    return (count == 1) ? WakeByAddressSingle(addr) : WakeByAddressAll(addr);
}

} // namespace concord::syscall::detail
