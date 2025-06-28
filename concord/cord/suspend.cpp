

#include "concord/cord/cord.hpp"
#include "concord/cord/handle.hpp"

namespace concord::cord {

auto suspend(Awaiter awaiter) -> void {
    Cord::self().suspend(std::move(awaiter));
}

auto yield() -> void {
    suspend([](CordHandle handle) { handle.resume(); });
}
} // namespace concord::cord