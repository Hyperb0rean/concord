#include "uring.hpp"

#include "liburing.h"

namespace concord::rt::io {

Uring::Uring() {}

auto Uring::spawn(ITask*) -> void {}

auto Uring::run() -> void {}
} // namespace concord::rt::io