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
#include "concord/os/wait/wait.hpp"
#include "concord/runtime/loop/loop.hpp"
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

auto cord_test() -> void {
    using namespace concord::cord; // NOLINT
    concord::runtime::loop::Loop rt;

    int counter = 0;

    go(rt, [&] {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Cord 1: " << counter++ << "\n";
            yield();
        }
    });

    go(rt, [&] {
        for (int i = 0; i < 3; ++i) {
            std::cout << "Cord 2: " << counter++ << "\n";
            go([&] { std::cout << "Inner Cord: " << counter++ << "\n"; });
            yield();
        }
    });

    rt.run();

    fmt::println("allocations: {}", allocated);
}

auto main() -> int {
    cord_test();
    wait_test();
}