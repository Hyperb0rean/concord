#pragma once

#include <span>

#include "concord/cord/runnable.hpp"

namespace concord::cord::context {
struct Context {
    void* rsp;

    auto make(std::span<std::byte> stack, IRunnable* fn) noexcept -> void;
    auto switch_to(Context& dest) noexcept -> void;
};
} // namespace concord::cord::context

extern "C" {
auto make_context(void* stack_bottom, void* thunk, void* fn) -> void*;
void context_switch(void** from, void** to);
}