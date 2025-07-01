#pragma once

#include "concord/cord/cord.hpp"
#include "mpsc_stack.hpp"

namespace concord::cord::sync {

class Event {
  public:
    auto wait() -> void {
        if (_wait_queue.is_closed()) {
            return;
        }
        Cord::self().suspend([&](CordHandle self) {
            if (!_wait_queue.push(self.get())) {
                // TODO: LIFO
                self.resume();
            }
        });
    }

    auto fire() -> void {
        _wait_queue.close([](rt::IntrusiveTask* handle) {
            static_cast<Cord*>(handle)->resume();
        });
    }

  private:
    detail::MPSCStack<rt::IntrusiveTask> _wait_queue;
};

} // namespace concord::cord::sync
