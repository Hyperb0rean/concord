#include <atomic>
#include <axis/monad/maybe.hpp>
#include <axis/monad/result.hpp>
#include <iostream>
#include <thread>

#include "concord/syscall/wait/wait.hpp"

auto main() -> int {
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