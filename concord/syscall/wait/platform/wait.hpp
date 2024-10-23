#pragma once

#include <cstddef>
#include <cstdint>

namespace concord::syscall::detail {
auto wait_timed(uint32_t* addr, uint32_t old, uint32_t millis) -> int;
auto wait(uint32_t* addr, uint32_t old) -> int;
auto wake(uint32_t* addr, size_t count) -> int;
} // namespace concord::syscall::detail