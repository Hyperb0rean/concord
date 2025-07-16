#pragma once

#include "concord/future/comb/with.hpp"

namespace concord::future {
template<typename S> //NOLINTNEXTLINE(readability-identifier-naming)
auto With(S state) {
    return comb::With<S> {std::move(state)};
}
} // namespace concord::future