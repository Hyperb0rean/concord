#pragma once

#include "concord/runtime/runtime.hpp"

namespace concord::future {

struct FutureLocal {
    rt::IRuntime* runtime;
};

} // namespace concord::future