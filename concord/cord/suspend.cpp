

#include "concord/cord/cord.hpp"
#include "concord/cord/handle.hpp"

namespace concord::cord {

auto suspend(IAwaiter* awaiter) -> void {
    Cord::self().suspend(awaiter);
}

auto yield() -> void {
    struct YieldAwaiter final: IAwaiter {
        auto await(CordHandle handle) -> CordHandle {
            handle.spawn();
            return CordHandle::invalid();
        }
    } awaiter;

    suspend(&awaiter);
}
} // namespace concord::cord