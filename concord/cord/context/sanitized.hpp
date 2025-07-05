#pragma once

#include "concord/cord/context/context.hpp"
#include "concord/cord/runnable.hpp"
#include "concord/cord/stack.hpp"

namespace concord::cord::context {
struct SanitizedContext: private Context {
    auto make(Stack stack, IRunnable* fn) noexcept -> void;
    auto switch_to(SanitizedContext& target) noexcept -> void;
    auto exit_to [[noreturn]] (SanitizedContext& target) noexcept -> void;
    auto before_run() noexcept -> void;

  private:
    const void* _stack;
    std::size_t _stack_size;
    void* _fake_stack;
    SanitizedContext* _from;
};
} // namespace concord::cord::context
