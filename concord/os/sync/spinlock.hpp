#pragma once

#include <atomic>
#include <cstdint>

namespace concord::os::sync {

// https://rigtorp.se/spinlock/

class SpinLock {
  public:
    auto lock() noexcept -> void;
    auto try_lock() noexcept -> bool;

    auto unlock() noexcept -> void;

  private:
    std::atomic_flag _lock {false};
};

} // namespace concord::os::sync