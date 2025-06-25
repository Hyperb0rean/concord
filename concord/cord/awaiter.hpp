#pragma once

#include <functional>

#include "concord/cord/handle.hpp"

namespace concord::cord {
using Awaiter = std::move_only_function<void(CordHandle)>;
}