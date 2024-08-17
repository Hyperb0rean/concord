#pragma once

#include <expected>
#include <system_error>
#include "../unit.hpp"

namespace axis::monad {

using Error = std::error_code;

template <typename T>
using Result = std::expected<T, Error>;

using Status = Result<Unit>;

}  // namespace axis::monad