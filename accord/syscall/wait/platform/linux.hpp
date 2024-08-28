#pragma once
#include <ctime>
#include "wait.hpp"

#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>

namespace {
// NOLINTNEXTLINE
int futex(unsigned int* uaddr, int op, int val, const struct timespec* timeout,
          int* uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, op, val, timeout, uaddr2, val3);
}
}  // namespace

namespace accord::syscall::detail {
auto WaitTimed(uint32_t* addr, uint32_t old, uint32_t millis) -> int {
  struct timespec timeout;
  timeout.tv_sec = millis / 1000;
  timeout.tv_nsec = (millis % 1000) * 1000'000;

  return futex(addr, FUTEX_WAIT_PRIVATE, old, &timeout, nullptr, 0);
}

auto Wait(uint32_t* addr, uint32_t old) -> int {
  return futex(addr, FUTEX_WAIT_PRIVATE, old, nullptr, nullptr, 0);
}

auto Wake(uint32_t* addr, size_t count) -> int {
  return futex(addr, FUTEX_WAKE_PRIVATE, (int)count, nullptr, nullptr, 0);
}

}  // namespace accord::syscall::detail
