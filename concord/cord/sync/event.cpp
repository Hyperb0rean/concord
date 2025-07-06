
#include "event.hpp"

namespace concord::cord::sync {
auto Event::wait() noexcept -> void {
    if (_wait_queue.is_closed()) {
        return;
    }

    struct EventAwaiter final: IAwaiter {
        auto await(CordHandle self) -> CordHandle final {
            if (!_event->_wait_queue.push(self.get())) {
                return self;
            }
            return CordHandle::invalid();
        }

        explicit EventAwaiter(Event* event) : _event(event) {}

        Event* _event;
    } awaiter {this};

    Cord::self().suspend(&awaiter);
}

auto Event::fire() noexcept -> void {
    _wait_queue.close([](rt::IntrusiveTask* handle) {
        static_cast<Cord*>(handle)->spawn();
    });
}

} // namespace concord::cord::sync
