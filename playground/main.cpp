
#include "concord/cord/go.hpp"
#include "concord/cord/suspend.hpp"
#include "concord/cord/sync/mutex.hpp"
#include "concord/future/get.hpp"
#include "concord/future/just.hpp"
#include "concord/future/map.hpp"
#include "concord/future/pipe.hpp"
#include "concord/future/ready.hpp"
#include "concord/future/with.hpp"
#include "concord/runtime/loop/loop.hpp"
#include "concord/runtime/thread/thread_pool.hpp"
#include "fmt/core.h"

std::size_t allocated;

auto operator new(std::size_t sz) -> void* {
    ++allocated;
    return malloc(sz);
}

auto operator new(std::size_t sz, std::align_val_t align) -> void* {
    ++allocated;
    return aligned_alloc(sz, static_cast<std::size_t>(align));
}

auto operator delete(void* ptr) noexcept -> void {
    free(ptr);
}

auto operator delete(void* ptr, std::size_t) noexcept -> void {
    free(ptr);
}

auto operator delete(void* ptr, std::align_val_t) noexcept -> void {
    free(ptr);
}

auto cord_test() -> void {
    using namespace concord::cord; // NOLINT
    concord::rt::thread::ThreadPool rt {2};

    sync::Mutex mu;
    int counter = 0;

    go(rt, [&] {
        for (int i = 0; i < 3; ++i) {
            std::lock_guard guard {mu};
            fmt::println("Cord 1: {}", counter++);
            yield();
        }
    });

    go(rt, [&] {
        std::lock_guard guard {mu};
        for (int i = 0; i < 3; ++i) {
            fmt::println("Cord 2: {}", counter++);
        }
    });

    rt.run();

    rt.stop();

    fmt::println("allocations: {}", allocated);
}

auto future_test() -> void {
    using namespace concord::future; //NOLINT
    concord::rt::loop::Loop rt {};

    auto f = Just() //
        | With(Runtime {&rt}) //
        | Map([](Unit) {
                 fmt::println("in loop");
                 return 1488;
             });

    auto val = Get(std::move(f));

    fmt::println("val: {}", val);
    assert(val == 1488);

    fmt::println("allocations: {}", allocated);
}

auto main() -> int {
    future_test();
}