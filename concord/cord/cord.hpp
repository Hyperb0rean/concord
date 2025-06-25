#pragma once

#include "concord/cord/awaiter.hpp"
#include "concord/cord/coroutine.hpp"
#include "concord/runtime/intrusive_task.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::cord {

class Cord:
    public runtime::IntrusiveTask,
    public axis::IntrusiveListNode<Cord> {
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

    auto with_stack(Stack stack) -> void;

    auto with_runtime(runtime::IRuntime* rt) -> void;
    auto runtime() const -> runtime::IRuntime*;

    auto suspend(Awaiter) -> void;

    auto spawn() -> void;
    auto switch_to() -> void;

    static auto self() -> Cord&;

    auto run() noexcept -> void final;

    static constexpr size_t stack_size = 4096 * 1024;

  private:
    Stack _stack;

    runtime::IRuntime* _runtime {nullptr};
    Coroutine _coroutine;
    Awaiter _awaiter;
};
} // namespace concord::cord