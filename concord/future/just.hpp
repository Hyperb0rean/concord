#pragma once

#include "concord/future/concepts.hpp"
#include "concord/future/thunk/just.hpp"

namespace concord::future {

template<typename V>

//NOLINTNEXTLINE(readability-identifier-naming)
auto Just() -> Future<V> auto {
    return thunk::Just {};
}

} // namespace concord::future