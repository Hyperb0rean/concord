#pragma once

#include <cstddef>
#include <cstdint>

namespace concord::syscall::detail {
auto WaitTimed(uint32_t* addr, uint32_t old, uint32_t millis) -> int;
auto Wait(uint32_t* addr, uint32_t old) -> int;
auto Wake(uint32_t* addr, size_t count) -> int;
}  // namespace concord::syscall::detail