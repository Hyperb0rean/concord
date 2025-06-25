
#include "cord.hpp"

#include <utility>

#include "current.hpp"

namespace concord::cord {

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

auto Cord::switch_to() -> void {
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
        delete this;
        Current::set(nullptr);
    } else {
        _awaiter(CordHandle {this});
    }
}

} // namespace concord::cord