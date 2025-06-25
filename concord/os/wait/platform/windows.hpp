#pragma once
#include <cstddef>
#include <cstdint>

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

namespace {

auto wait_timed(uint32_t* addr, uint32_t expected, uint32_t millis) -> int {
    return WaitOnAddress(addr, expected, 4, millis);
}

auto wait(uint32_t* addr, uint32_t old) -> int {
    return wait_timed(addr, old, /*millis=*/0);
}

auto wake(uint32_t* addr, std::size_t count) -> int {
    return (count == 1) ? WakeByAddressSingle(addr) : WakeByAddressAll(addr);
}

} // namespace