#pragma once

#include "concord/cord/awaiter.hpp"
#include "concord/cord/coroutine.hpp"
#include "concord/os/mmap/mmap.hpp"
#include "concord/runtime/intrusive_task.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::cord {

class Cord: public rt::IntrusiveTask {
  public:
    template<std::invocable F>
    Cord(F&& fn) : _coroutine(std::forward<F>(fn)) { // NOLINT
    }

    // Non-movable
    Cord(Cord&& other) = delete;
    auto operator=(Cord&& other) -> Cord& = delete;
    // Non-copyable
    Cord(const Cord&) = delete;
    auto operator=(const Cord&) -> Cord& = delete;

    ~Cord() = default;

    auto with_stack(os::MemoryAllocation stack) -> void;

    auto with_runtime(rt::IRuntime* rt) -> void;
    auto runtime() const -> rt::IRuntime*;

    auto suspend(IAwaiter*) -> void;

    auto spawn() -> void;
    auto resume() -> void;

    static auto self() -> Cord&;

    auto run() noexcept -> void final;

    static constexpr size_t stack_size = 4096 * 1024;

  private:
    os::MemoryAllocation _stack;

    rt::IRuntime* _runtime {nullptr};
    Coroutine _coroutine;
    IAwaiter* _awaiter;
};
} // namespace concord::cord