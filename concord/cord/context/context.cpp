#include "context.hpp"

#include "concord/cord/runnable.hpp"

namespace concord::cord::context {

static auto context_thunk [[noreturn]] (
    void*,
    void*,
    void*,
    void*,
    void*,
    void*,
    void* stack_arg
) noexcept -> void {
    auto runnable = static_cast<IRunnable*>(stack_arg);
    runnable->run();
}

auto Context::make(std::span<std::byte> stack, IRunnable* fn) noexcept -> void {
    rsp = make_context(
        &stack.back(),
        reinterpret_cast<void*>(&context_thunk),
        fn
    );
}

auto Context::switch_to(Context& target) noexcept -> void {
    context_switch(&rsp, &target.rsp);
}

} // namespace concord::cord::context