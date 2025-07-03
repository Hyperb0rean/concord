
#include "event.hpp"

namespace concord::cord::sync {
auto Event::wait() noexcept -> void {
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

auto Event::fire() noexcept -> void {
    _wait_queue.close([](rt::IntrusiveTask* handle) {
        static_cast<Cord*>(handle)->spawn();
    });
}

} // namespace concord::cord::sync
