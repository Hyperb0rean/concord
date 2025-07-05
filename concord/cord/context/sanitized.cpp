#include "concord/cord/context/sanitized.hpp"

#include <sanitizer/asan_interface.h>

#include "concord/cord/runnable.hpp"
#include "concord/cord/stack.hpp"

namespace concord::cord::context {

auto SanitizedContext::make(Stack stack, IRunnable* fn) noexcept -> void {
    Context::make(stack, fn);
    _stack = stack.data();
    _stack_size = stack.size();
}

auto SanitizedContext::switch_to(SanitizedContext& target) noexcept -> void {
    target._from = this;
    __sanitizer_start_switch_fiber(
        &_fake_stack,
        target._stack,
        target._stack_size
    );

    Context::switch_to(target);

    __sanitizer_finish_switch_fiber(
        _fake_stack,
        &(_from->_stack),
        &(_from->_stack_size)
    );
}

auto SanitizedContext::exit_to(SanitizedContext& target) noexcept -> void {
    target._from = this;
    __sanitizer_start_switch_fiber(nullptr, target._stack, target._stack_size);
    Context::exit_to(target);
}

auto SanitizedContext::before_run() noexcept -> void {
    __sanitizer_finish_switch_fiber(
        nullptr,
        &(_from->_stack),
        &(_from->_stack_size)
    );
    Context::before_run();
}

} // namespace concord::cord::context
