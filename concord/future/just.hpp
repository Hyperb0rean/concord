#pragma once

#include "concord/future/concepts.hpp"
#include "concord/future/thunk/just.hpp"

namespace concord::future {

//NOLINTNEXTLINE(readability-identifier-naming)
auto Just() -> Future<Unit, Unit> auto {
    return thunk::Just {};
}

} // namespace concord::future