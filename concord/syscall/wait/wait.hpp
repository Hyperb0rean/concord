#pragma once

#include <atomic>
#include <cstdint>

namespace concord::syscall {

auto wait(
    std::atomic<uint32_t>* atomic, //
    uint32_t old, //
    std::memory_order mo = std::memory_order::seq_cst
) -> void;

auto wait_timed(
    std::atomic<uint32_t>* atomic, //
    uint32_t old, //
    std::chrono::milliseconds timeout, //
    std::memory_order mo = std::memory_order::seq_cst
) -> bool;

auto wake_one(std::atomic<uint32_t>* atomic) -> void;
auto wake_all(std::atomic<uint32_t>* atomic) -> void;

} // namespace concord::syscall