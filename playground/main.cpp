#include <atomic>
#include <axis/monad/maybe.hpp>
#include <axis/monad/result.hpp>

#include "concord/syscall/wait/wait.hpp"

auto main() -> int {
    std::atomic<uint32_t> flag {0};
    concord::syscall::wait(&flag, 0, std::memory_order::seq_cst);
}