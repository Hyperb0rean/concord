#include "coroutine.hpp"

namespace concord::cord {

auto Coroutine::with_stack(Stack stack) -> void {
    _callee_context.make(stack, this);
}

auto Coroutine::resume() -> void {
    _caller_context.switch_to(_callee_context);
}

auto Coroutine::suspend() -> void {
    _callee_context.switch_to(_caller_context);
}

auto Coroutine::is_completed() const -> bool {
    return _is_completed;
}

auto Coroutine::run() noexcept -> void {
    _callee_context.before_run();
    _func();
    _is_completed = true;

    _callee_context.exit_to(_caller_context);
}
} // namespace concord::cord
