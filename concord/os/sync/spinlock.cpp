
#include "concord/os/sync/spinlock.hpp"

#include "concord/os/sync/spinner.hpp"

namespace concord::os::sync {

auto SpinLock::lock() noexcept -> void {
    Spinner<> spinner;
    while (_lock.test_and_set(std::memory_order::acquire)) {
        while (_lock.test(std::memory_order::relaxed)) {
            spinner();
        }
    }
}

auto SpinLock::try_lock() noexcept -> bool {
    return !_lock.test(std::memory_order::relaxed)
        && !_lock.test_and_set(std::memory_order::acquire);
}

auto SpinLock::unlock() noexcept -> void {
    _lock.clear(std::memory_order::release);
}

} // namespace concord::os::sync