

#include "concord/cord/cord.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::cord {
template<typename F>
auto go(runtime::IRuntime& rt, F&& fn) -> void {
    auto stack = StackAllocator::allocate(Cord::stack_size);
    auto view = stack.view();
    auto* task = new (view.data() + view.size() - sizeof(Cord))
        concord::cord::Cord {std::forward<F>(fn)};
    task->with_stack(stack);
    task->with_runtime(&rt);
    task->spawn();
}

template<typename F>
auto go(F&& fn) -> void {
    go(*Cord::self().runtime(), std::forward<F>(fn));
}
} // namespace concord::cord