#include <atomic>
#include <axis/monad/maybe.hpp>
#include <axis/monad/result.hpp>
#include <iostream>
#include <thread>

#include "concord/cord/context/context.hpp"
#include "concord/cord/cord.hpp"
#include "concord/cord/go.hpp"
#include "concord/cord/stack.hpp"
#include "concord/cord/suspend.hpp"
#include "concord/cord/sync/event.hpp"
#include "concord/cord/sync/mutex.hpp"
#include "concord/os/sync/wait_group.hpp"
#include "concord/os/wait/wait.hpp"
#include "concord/runtime/loop/loop.hpp"
#include "concord/runtime/thread/thread_pool.hpp"
#include "fmt/core.h"

auto wait_test() -> void {
    std::atomic<uint64_t> flag {0};
    auto producer = std::thread([&] {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        auto&& token =
            concord::os::prepare_wake(concord::os::AtomicRefUint64Low {flag});
        std::cout << "Awaken" << std::endl;
        flag.store(1);
        concord::os::wake_one(std::move(token));
    });
    std::cout << "Sleept" << std::endl;
    concord::os::wait(
        concord::os::AtomicRefUint64Low {flag},
        0,
        std::memory_order::seq_cst
    );
    producer.join();
}

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

auto main() -> int {
    cord_test();
    // wait_test();
}