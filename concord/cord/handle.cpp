#include "handle.hpp"

#include <cassert>
#include <utility>

#include "cord.hpp"

namespace concord::cord {
auto CordHandle::release() -> Cord* {
    assert(is_valid());
    return std::exchange(_cord, nullptr);
}

auto CordHandle::spawn() -> void {
    release()->spawn();
}

auto CordHandle::switch_to() -> void {
    release()->switch_to();
}

auto CordHandle::get() -> Cord* {
    return _cord;
}

} // namespace concord::cord