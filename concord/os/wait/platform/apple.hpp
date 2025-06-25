#pragma once
#include <cstddef>
#include <cstdint>

// NOLINTNEXTLINE
extern "C" int __ulock_wait(
    uint32_t operation,
    void* addr,
    uint64_t value,
    uint32_t timeout
); /* timeout is specified in microseconds */

// NOLINTNEXTLINE
extern "C" int
__ulock_wake(uint32_t operation, void* addr, uint64_t wake_value); // NOLINT

#define UL_COMPARE_AND_WAIT 1
#define ULF_WAKE_ALL 0x00000100

namespace concord::os::detail {

inline auto wait_timed(uint32_t* addr, uint32_t expected, uint32_t millis)
    -> int {
    return __ulock_wait(UL_COMPARE_AND_WAIT, addr, expected, millis * 1000);
}

inline auto wait(uint32_t* addr, uint32_t old) -> int {
    return wait_timed(addr, old, /*millis=*/0);
}

inline auto wake(uint32_t* addr, size_t count) -> int {
    return __ulock_wake(
        UL_COMPARE_AND_WAIT | ((count == 1) ? 0 : ULF_WAKE_ALL),
        addr,
        0
    );
}

} // namespace concord::os::detail
