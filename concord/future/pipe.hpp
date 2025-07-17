#pragma once

#include "concord/future/concepts.hpp"

namespace concord::future {

template<Thunk F, typename C>
auto operator|(F&& f, C c) {
    return std::move(c).then(std::move(f));
}

} // namespace concord::future