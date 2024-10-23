#pragma once

#include <variant>

namespace axis::monad {

using Unit = std::monostate;

static constexpr Unit unit = Unit {};

} // namespace axis::monad