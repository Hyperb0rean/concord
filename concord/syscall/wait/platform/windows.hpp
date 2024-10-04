#pragma once
#include "wait.hpp"
#include "synchapi.h"

namespace concord::syscall::detail {

auto WaitTimed(uint32_t* addr, uint32_t expected, uint32_t millis) -> int {
  return WaitOnAddress(addr, expected, 4, millis);
}

auto Wait(uint32_t* addr, uint32_t old) -> int {
  return WaitTimed(addr, old, /*millis=*/0);
}

auto Wake(uint32_t* addr, size_t count) -> int {
  return (count == 1) ? WakeByAddressSingle(addr) : WakeByAddressAll(addr);
}

}  // namespace concord::syscall::detail
