
#include "platform/wait.hpp"
#ifdef LINUX
#include "platform/linux.hpp"
#elif APPLE
#include "platform/apple.hpp"
#else
#include "platform/windows.hpp"
#endif

#include "wait.hpp"

namespace concord::syscall {
auto Wait(std::atomic<uint32_t>* atomic, uint32_t old) -> void;
auto WaitTimed(std::atomic<uint32_t>* atomic, uint32_t old,
               std::chrono::milliseconds timeout) -> bool;

auto WakeOne(std::atomic<uint32_t>* atomic) -> void;
auto WakeAll(std::atomic<uint32_t>* atomic) -> void;
}  // namespace concord::syscall