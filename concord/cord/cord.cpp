
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

auto Cord::with_runtime(rt::IRuntime* rt) -> void {
    _runtime = rt;
}

auto Cord::runtime() const -> rt::IRuntime* {
    return _runtime;
}

auto Cord::suspend(IAwaiter* awaiter) -> void {
    _awaiter = awaiter;
    _coroutine.suspend();
    Current::set(this);
}

auto Cord::spawn() -> void {
    _runtime->spawn(this);
}

auto Cord::resume() -> void {
    struct Awaiter final: IAwaiter {
        auto await(CordHandle) -> CordHandle final {
            return _callee;
        }

        explicit Awaiter(CordHandle callee) : _callee(callee) {}

        CordHandle _callee;
    } awaiter {CordHandle {this}};

    Cord::self().suspend(&awaiter);
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
        auto handle = _awaiter->await(CordHandle {this});
        if (handle.is_valid()) {
            handle.release()->run();
        }
    }
}

} // namespace concord::cord