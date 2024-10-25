#pragma once
#include "wait.hpp"

#ifdef WIN32
    #include "synchapi.h"
#else
int WaitOnAddress( // NOLINT
    void* addr,
    uint32_t expected,
    size_t address_size,
    uint32_t millis
);

int WakeByAddressSingle(void* addr); //NOLINT
int WakeByAddressAll(void* addr); //NOLINT
#endif

namespace concord::syscall::detail {

inline auto wait_timed(uint32_t* addr, uint32_t expected, uint32_t millis)
    -> int {
    return WaitOnAddress(addr, expected, 4, millis);
}

inline auto wait(uint32_t* addr, uint32_t old) -> int {
    return wait_timed(addr, old, /*millis=*/0);
}

inline auto wake(uint32_t* addr, size_t count) -> int {
    return (count == 1) ? WakeByAddressSingle(addr) : WakeByAddressAll(addr);
}

} // namespace concord::syscall::detail