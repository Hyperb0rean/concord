#pragma once

#include "axis/container/intrusive_list.hpp"
#include "concord/runtime/task.hpp"

namespace concord::runtime {
struct IntrusiveTask: public ITask, axis::IntrusiveListNode<IntrusiveTask> {
    virtual ~IntrusiveTask() = default;
};
} // namespace concord::runtime