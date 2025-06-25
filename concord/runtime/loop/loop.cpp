#include "loop.hpp"

namespace concord::runtime::loop {
void Loop::spawn(ITask* task) {
    _tasks.push_back(static_cast<IntrusiveTask*>(task));
}

auto Loop::run() -> std::size_t {
    size_t runned = 0;
    while (!_tasks.is_empty()) {
        auto* task = _tasks.pop_front();
        task->run();
        ++runned;
    }
    return runned;
}
} // namespace concord::runtime::loop