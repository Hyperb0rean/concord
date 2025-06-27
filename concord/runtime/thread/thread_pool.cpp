

#include "concord/runtime/thread/thread_pool.hpp"

#include "concord/runtime/task.hpp"
#include "fmt/base.h"

namespace {
thread_local concord::rt::thread::ThreadPool* pool = nullptr;
}

namespace concord::rt::thread {

ThreadPool::~ThreadPool() {
    assert(_stopped);
}

auto ThreadPool::run() -> void {
    _workers.reserve(_threads);
    for (auto& worker : _workers) {
        worker = std::thread([&] {
            pool = this;
            while (auto* task = _tasks.pop()) {
                task->run();
            }
        });
    }
}

auto ThreadPool::spawn(ITask* task) -> void {
    _tasks.push(static_cast<IntrusiveTask*>(task));
}

auto ThreadPool::current() -> ThreadPool* {
    return pool;
}

auto ThreadPool::stop() -> void {
    _tasks.close();
    for (auto& worker : _workers) {
        worker.join();
    }
    _stopped = true;
}
} // namespace concord::rt::thread