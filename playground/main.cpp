#include <atomic>
#include <axis/monad/maybe.hpp>
#include <axis/monad/result.hpp>
#include <iostream>
#include <thread>

#include "concord/cord/context/context.hpp"
#include "concord/cord/cord.hpp"
#include "concord/cord/coroutine.hpp"
#include "concord/cord/runnable.hpp"
#include "concord/cord/stack.hpp"
#include "concord/runtime/loop/loop.hpp"
#include "concord/syscall/wait/wait.hpp"
#include "fmt/core.h"

auto wait_test() -> void {
    std::atomic<uint64_t> flag {0};
    auto producer = std::thread([&] {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        auto&& token = concord::syscall::prepare_wake(
            concord::syscall::AtomicRefUint64Low {flag}
        );
        std::cout << "Awaken" << std::endl;
        flag.store(1);
        concord::syscall::wake_one(std::move(token));
    });
    std::cout << "Sleept" << std::endl;
    concord::syscall::wait(
        concord::syscall::AtomicRefUint64Low {flag},
        0,
        std::memory_order::seq_cst
    );
    producer.join();
}

class ExampleCoroutine: public concord::cord::IRunnable {
  public:
    explicit ExampleCoroutine(int& counter) : _counter(counter) {}

    auto run [[noreturn]] () noexcept -> void final {
        while (true) {
            std::cout << "Coroutine: " << _counter++ << "\n";
            context.switch_to(*previous);
        }
    }

    concord::cord::context::Context context;
    concord::cord::context::Context* previous;

  private:
    int& _counter;
};

auto context_test() -> void {
    auto stack = concord::cord::StackAllocator::allocate(1024 * 1024);

    int counter = 0;
    ExampleCoroutine coro(counter);

    coro.context.make(stack.view(), &coro);

    concord::cord::context::Context main_context;
    coro.previous = &main_context;
}

auto context_test2() -> void {
    auto stack = concord::cord::StackAllocator::allocate(1024 * 1024);

    int counter = 0;

    concord::cord::Coroutine coro = [&]() {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Coroutine: " << counter++ << "\n";
            coro.suspend();
        }
    };

    coro.with_stack(stack.view());

    for (int i = 0; i < 3; ++i) {
        std::cout << "Main: " << counter << "\n";
        coro.resume();
    }
}

auto cord_test() -> void {
    using namespace concord::cord; // NOLINT
    concord::runtime::loop::Loop rt;

    int counter = 0;

    concord::cord::Cord task = [&]() {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Cord: " << counter++ << "\n";
            Cord::self().suspend([](CordHandle handle) { handle.spawn(); });
        }
    };

    task.with_runtime(&rt);
    rt.spawn(&task);

    rt.run();
}

auto main() -> int {
    cord_test();
}