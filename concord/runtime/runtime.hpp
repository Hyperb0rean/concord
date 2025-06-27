#pragma once

#include "concord/runtime/task.hpp"

namespace concord::rt {
struct IRuntime {
    virtual auto spawn(ITask*) -> void = 0;

  protected:
    virtual ~IRuntime() = default;
};
} // namespace concord::runtime