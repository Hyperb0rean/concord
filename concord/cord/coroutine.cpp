#include "coroutine.hpp"

#include <utility>

namespace concord::cord {

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
    _func();
    _is_completed = true;

    _callee_context.switch_to(_caller_context);
    std::unreachable();
}
} // namespace concord::cord
