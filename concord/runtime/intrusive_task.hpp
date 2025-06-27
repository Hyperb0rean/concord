#pragma once

#include "axis/container/intrusive_list.hpp"
#include "concord/runtime/task.hpp"

namespace concord::rt {
struct IntrusiveTask: public ITask, axis::IntrusiveListNode<IntrusiveTask> {
    virtual ~IntrusiveTask() = default;
};
} // namespace concord::rt