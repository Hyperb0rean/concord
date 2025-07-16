#pragma once

#include "concord/future/concepts.hpp"
#include "concord/future/thunk/ready.hpp"

namespace concord::future {

template<typename V>

//NOLINTNEXTLINE(readability-identifier-naming)
auto Ready(V value) -> Future<V> auto {
    return thunk::Ready<V> {std::move(value)};
}

} // namespace concord::future