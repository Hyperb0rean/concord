#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <span>

namespace concord::syscall {

class MemoryAllocation: private std::span<std::byte> {
  public:
    static auto allocate(std::size_t size, std::size_t align)
        -> MemoryAllocation;
    static auto deallocate(MemoryAllocation&&) noexcept -> void;

    auto view() const noexcept -> std::span<std::byte> {
        return {data(), size()};
    }

    MemoryAllocation() = default;

  private:
    MemoryAllocation(std::byte* ptr, std::size_t size, std::size_t align) :
        std::span<std::byte> {ptr, size},
        _align {align} {}

    auto protect() -> void;

    std::size_t _align;
};

extern std::size_t page_size;

} // namespace concord::syscall