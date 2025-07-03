#pragma once

#include "concord/cord/cord.hpp"
#include "concord/cord/sync/mpsc_stack.hpp"

namespace concord::cord::sync {

class Event {
  public:
    auto wait() noexcept -> void;
    auto fire() noexcept -> void;

  private:
    detail::MPSCStack<rt::IntrusiveTask> _wait_queue;
};

} // namespace concord::cord::sync
