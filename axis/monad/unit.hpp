#pragma once

#include <variant>

namespace axis::monad {

using Unit = std::monostate;

static constexpr Unit unit = Unit{};  // NOLINT

}  // namespace axis::monad