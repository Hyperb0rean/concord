#pragma once

#include "awaiter.hpp"

namespace concord::cord {

auto suspend(Awaiter) -> void;

void yield();

} // namespace concord::cord