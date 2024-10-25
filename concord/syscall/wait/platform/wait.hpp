#pragma once

#include <cstddef>
#include <cstdint>

namespace concord::syscall::detail {
inline auto wait_timed(uint32_t* addr, uint32_t old, uint32_t millis) -> int;
inline auto wait(uint32_t* addr, uint32_t old) -> int;
inline auto wake(uint32_t* addr, size_t count) -> int;
} // namespace concord::syscall::detail