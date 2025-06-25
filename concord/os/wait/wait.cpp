

#include "concord/os/wait/atomic/wait.hpp"

#include "concord/os/wait/atomic/u32.hpp"
#include "concord/os/wait/atomic/u64.hpp"
#include "concord/os/wait/wait.hpp"

#ifdef LINUX
    #include "platform/linux.hpp"
#elif APPLE
    #include "platform/apple.hpp"
#elif WIN32
    #include "platform/windows.hpp"
#endif

namespace concord::os {

auto wait(std::atomic<uint32_t>& atomic, uint32_t old, std::memory_order mo)
    -> uint32_t {
    return atomic::wait(atomic::AtomicRefUint32 {atomic}, old, mo);
}

auto wait(
    AtomicRefUint64High atom_ref, //
    uint32_t old, //
    std::memory_order mo
) -> uint64_t {
    return atomic::wait(atom_ref, old, mo);
}

auto wait(
    AtomicRefUint64Low atom_ref, //
    uint32_t old, //
    std::memory_order mo
) -> uint64_t {
    return atomic::wait(atom_ref, old, mo);
}

// Unimplemented
auto wait_timed(
    std::atomic<uint32_t>*,
    uint32_t,
    std::chrono::milliseconds,
    std::memory_order
) -> void {}

auto wake_one(WakeToken token) -> void {
    wake(token.location, 1);
}

auto wake_all(WakeToken token) -> void {
    wake(token.location, std::numeric_limits<uint32_t>::max());
}
} // namespace concord::os