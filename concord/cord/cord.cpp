
#include "cord.hpp"

#include <utility>

#include "current.hpp"

namespace concord::cord {

auto Cord::with_stack(os::MemoryAllocation stack) -> void {
    _stack = std::move(stack);
    auto sz = _stack.view().size();
    sz -= sizeof(Cord);
    _coroutine.with_stack(_stack.view().subspan(0, sz));
}

auto Cord::with_runtime(runtime::IRuntime* rt) -> void {
    _runtime = rt;
}

auto Cord::runtime() const -> runtime::IRuntime* {
    return _runtime;
}

auto Cord::suspend(Awaiter awaiter) -> void {
    _awaiter = std::move(awaiter);
    _coroutine.suspend();
    Current::set(this);
}

auto Cord::spawn() -> void {
    _runtime->spawn(this);
}

auto Cord::resume() -> void {
    // TODO
    // run() ?
}

auto Cord::self() -> Cord& {
    return *Current::get();
}

auto Cord::run() noexcept -> void {
    Current::set(this);
    _coroutine.resume();

    if (_coroutine.is_completed()) {
        auto stack = _stack;
        this->~Cord();
        StackAllocator::dellocate(stack);
        Current::set(nullptr);
    } else {
        _awaiter(CordHandle {this});
    }
}

} // namespace concord::cord