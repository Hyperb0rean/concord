#pragma once

#include <cassert>
#include <mutex>

#include "axis/container/intrusive_list.hpp"
#include "concord/os/sync/mutex.hpp"
#include "concord/os/wait/wait.hpp"

namespace concord::rt::thread {

template<typename T>
class UnboundedBlockingQueue {
  public:
    auto push(T* item) -> void {
        std::unique_lock guard {_mutex};
        _queue.push_back(item);
        auto&& token = os::prepare_wake(_seq);
        _seq.fetch_add(1, std::memory_order::relaxed);
        os::wake_one(std::move(token));
    }

    auto pop() -> T* {
        std::unique_lock guard {_mutex};
        uint32_t seq = _seq.load(std::memory_order::relaxed);
        while (_queue.is_empty() && !_closed) {
            guard.unlock();
            seq = os::wait(_seq, seq, std::memory_order::relaxed);
            guard.lock();
        }
        if (_queue.is_empty()) {
            return nullptr;
        }
        auto* item = _queue.pop_front();
        return item;
    }

    auto close() -> void {
        std::unique_lock guard {_mutex};
        _closed = true;
        auto&& token = os::prepare_wake(_seq);
        _seq.fetch_add(1, std::memory_order::relaxed);
        os::wake_all(std::move(token));
    }

  private:
    static constexpr std::uint32_t sentinel =
        std::numeric_limits<std::uint32_t>::max();
    static constexpr std::size_t cache_line_lize =
        std::hardware_destructive_interference_size;

    std::atomic<std::uint32_t> _seq {0};
    bool _closed {false};
    axis::IntrusiveList<T> _queue;

    alignas(cache_line_lize * 2) os::sync::Mutex _mutex;
};

} // namespace concord::rt::thread