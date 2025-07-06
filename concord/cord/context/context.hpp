#pragma once

#include "concord/cord/runnable.hpp"
#include "concord/cord/stack.hpp"

namespace concord::cord::context {
struct Context {
    auto make(Stack stack, IRunnable* fn) noexcept -> void;
    auto switch_to(Context& target) noexcept -> void;
    auto exit_to [[noreturn]] (Context& target) noexcept -> void;
    auto before_run() noexcept -> void;

    void* rsp;
};
} // namespace concord::cord::context

extern "C" {
auto make_context(void* stack_bottom, void* thunk, void* fn) -> void*;
void context_switch(void** from, void** to);
}