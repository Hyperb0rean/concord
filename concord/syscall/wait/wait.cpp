

#include "wait.hpp"

#include "platform/wait.hpp"

#ifdef LINUX
    #include "platform/linux.hpp"
#elif APPLE
    #include "platform/apple.hpp"
#else
    #include "platform/windows.hpp"
#endif

namespace concord::syscall {

auto wait(std::atomic<uint32_t>* atomic, uint32_t old, std::memory_order mo)
    -> void {
    detail::wait(&*reinterpret_cast<uint32_t*>(atomic), old);
}

auto wait_timed(
    std::atomic<uint32_t>* atomic,
    uint32_t old,
    std::chrono::milliseconds timeout,
    std::memory_order mo
) -> void {
    detail::wait_timed(
        &*reinterpret_cast<uint32_t*>(atomic),
        old,
        timeout.count()
    );
}

auto wake_one(std::atomic<uint32_t>* atomic) -> void {
    detail::wake(&*reinterpret_cast<uint32_t*>(atomic), 1);
}

auto wake_all(std::atomic<uint32_t>* atomic) -> void {
    detail::wake(&*reinterpret_cast<uint32_t*>(atomic), INT_MAX);
}
} // namespace concord::syscall