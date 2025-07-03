#pragma once

#include "concord/cord/cord.hpp"
#include "concord/cord/sync/mpsc_queue.hpp"
#include "concord/os/sync/spinlock.hpp"

namespace concord::cord::sync {

class Mutex {
  public:
    auto lock() noexcept -> void;

    auto try_lock() noexcept -> bool;

    auto unlock() noexcept -> void;

  private:
    os::sync::SpinLock _lock;
    Cord* _combiner {nullptr};
    Cord* _runner {nullptr};
    detail::MPSCQueue<rt::IntrusiveTask> _wait_queue;
};

} // namespace concord::cord::sync
