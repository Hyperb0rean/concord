#pragma once

#include <cstddef>
#include <thread>
#include <vector>

#include "concord/runtime/intrusive_task.hpp"
#include "concord/runtime/runtime.hpp"
#include "concord/runtime/thread/queue.hpp"

namespace concord::rt::thread {
class ThreadPool final: public IRuntime {
  public:
    explicit ThreadPool(std::size_t threads) : _threads(threads) {}

    ~ThreadPool();

    // Non-copyable
    ThreadPool(const ThreadPool&) = delete;
    auto operator=(const ThreadPool&) -> ThreadPool& = delete;

    // Non-movable
    ThreadPool(ThreadPool&&) = delete;
    auto operator=(ThreadPool&&) -> ThreadPool& = delete;

    auto run() -> void;

    auto spawn(ITask*) -> void final;

    auto static current() -> ThreadPool*;

    auto stop() -> void;

  private:
    bool _stopped {false};

    std::size_t _threads;
    std::vector<std::thread> _workers;

    UnboundedBlockingQueue<IntrusiveTask> _tasks;
};
} // namespace concord::rt::thread