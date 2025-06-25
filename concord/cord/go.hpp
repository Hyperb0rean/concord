

#include "concord/cord/cord.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::cord {
template<typename F>
auto go(runtime::IRuntime& rt, F&& fn) -> void {
    auto stack = StackAllocator::allocate(Cord::stack_size, alignof(Cord));
    auto* task =
        new (&stack.view().back()) concord::cord::Cord {std::forward<F>(fn)};
    task->with_stack(stack);
    task->with_runtime(&rt);
    task->spawn();
}

template<typename F>
auto go(F&& fn) -> void {
    go(*Cord::self().runtime(), std::forward<F>(fn));
}
} // namespace concord::cord