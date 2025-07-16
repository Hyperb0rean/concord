#pragma once
#include "concord/runtime/runtime.hpp"

namespace concord::rt::io {
class Uring: public IRuntime {
  public:
    Uring();

    // Non-copyable
    Uring(const Uring&) = delete;
    auto operator=(const Uring&) -> Uring& = delete;

    // Non-movable
    Uring(Uring&&) = delete;
    auto operator=(Uring&&) -> Uring& = delete;

    auto spawn(ITask*) -> void final;

    auto run() -> void;
};
} // namespace concord::rt::io