#include "concord/cord/sync/mutex.hpp"

namespace concord::cord::sync {
auto Mutex::lock() noexcept -> void {
    if (try_lock()) {
        _runner = &Cord::self();
        return;
    }
    Cord::self().suspend([this](CordHandle self) {
        _wait_queue.push(static_cast<rt::IntrusiveTask*>(self.get()));
    });
}

auto Mutex::try_lock() noexcept -> bool {
    return _lock.try_lock();
}

auto Mutex::unlock() noexcept -> void {
    if (_combiner == nullptr) {
        while (auto* handle = _wait_queue.pop()) {
            _combiner = &Cord::self();
            _runner = static_cast<Cord*>(handle);
            _runner->resume();
            _runner->spawn();
            _combiner = nullptr;
        }
        // <-- preempts: Here is the problem with queue may not be already empty.
        _lock.unlock();
        return;
    }
    _combiner->resume();
}
} // namespace concord::cord::sync