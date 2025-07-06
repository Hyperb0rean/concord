#pragma once

#include "awaiter.hpp"

namespace concord::cord {

auto suspend(IAwaiter*) -> void;

void yield();

} // namespace concord::cord