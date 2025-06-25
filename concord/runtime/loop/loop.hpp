#pragma once

#include <cstddef>

#include "axis/container/intrusive_list.hpp"
#include "concord/runtime/intrusive_task.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::runtime::loop {
class Loop: public IRuntime {
  public:
    Loop() = default;

    // Non-copyable
    Loop(const Loop&) = delete;
    auto operator=(const Loop&) -> Loop& = delete;

    // Non-movable
    Loop(Loop&&) = delete;
    auto operator=(Loop&&) -> Loop& = delete;

    auto spawn(ITask*) -> void final;

    auto run() -> std::size_t;

  private:
    axis::IntrusiveList<IntrusiveTask> _tasks;
};
} // namespace concord::runtime::loop