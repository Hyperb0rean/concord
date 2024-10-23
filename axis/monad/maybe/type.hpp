#pragma once

#include "optional"

namespace axis::monad {
template<typename T>
using Maybe = std::optional<T>;
}