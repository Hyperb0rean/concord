#include "concord/cord/sync/mutex.hpp"

namespace concord::cord::sync {
auto Mutex::lock() noexcept -> void {
    if (try_lock()) {
        _runner = &Cord::self();
        return;
    }

    struct LockAwaiter final: IAwaiter {
        auto await(CordHandle self) -> CordHandle final {
            _mutex->_wait_queue.push(
                static_cast<rt::IntrusiveTask*>(self.get())
            );
            return CordHandle::invalid();
        }

        explicit LockAwaiter(Mutex* mutex) : _mutex(mutex) {}

        Mutex* _mutex;
    } awaiter {this};

    Cord::self().suspend(&awaiter);
}

auto Mutex::try_lock() noexcept -> bool {
    return _lock.try_lock();
}

auto Mutex::unlock() noexcept -> void {
    if (_combiner == nullptr) {
        _combiner = &Cord::self();
        while (auto* handle = _wait_queue.pop()) {
            _runner = static_cast<Cord*>(handle);
            _runner->resume();
            _runner->spawn();
        }
        _combiner = nullptr;
        // <-- preempts: Here is the problem with queue may not be already empty.
        _lock.unlock();
        return;
    }
    _combiner->resume();
}
} // namespace concord::cord::sync