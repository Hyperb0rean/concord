#pragma once

#include <functional>

#include "concord/cord/context/context.hpp"
#include "concord/cord/runnable.hpp"

namespace concord::cord {

class Coroutine: public IRunnable {
#if defined(__SANITIZE_ADDRESS__)
    using Context = context::Sanitized;
#else
    using Context = context::Context;
#endif

  public:
    template<std::invocable F>
    Coroutine(F&& fn) : _func(std::forward<F>(fn)) {} // NOLINT

    auto with_stack(Stack stack) -> void;

    auto resume() -> void;

    auto suspend() -> void;

    auto is_completed() const -> bool;

    auto run [[noreturn]] () noexcept -> void final;

  private:
    bool _is_completed {false};

    std::move_only_function<void()> _func;

    Context _callee_context;
    Context _caller_context;
};

} // namespace concord::cord