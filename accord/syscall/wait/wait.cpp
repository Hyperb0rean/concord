
#include "platform/wait.hpp"
#include "wait.hpp"

namespace accord::syscall {
auto Wait(std::atomic<uint32_t>* atomic, uint32_t old) -> void;
auto WaitTimed(std::atomic<uint32_t>* atomic, uint32_t old,
               std::chrono::milliseconds timeout) -> bool;

auto WakeOne(std::atomic<uint32_t>* atomic) -> void;
auto WakeAll(std::atomic<uint32_t>* atomic) -> void;
}  // namespace accord::syscall