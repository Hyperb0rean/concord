#pragma once

#include <atomic>
#include <cstdint>
namespace accord::syscall {

auto Wait(std::atomic<uint32_t>* atomic,  //
          uint32_t old,                   //
          std::memory_order mo = std::memory_order::seq_cst) -> void;

auto WaitTimed(std::atomic<uint32_t>* atomic,      //
               uint32_t old,                       //
               std::chrono::milliseconds timeout,  //
               std::memory_order mo = std::memory_order::seq_cst) -> bool;

auto WakeOne(std::atomic<uint32_t>* atomic) -> void;
auto WakeAll(std::atomic<uint32_t>* atomic) -> void;

}  // namespace accord::syscall