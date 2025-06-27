#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <span>

namespace concord::os {

class MemoryAllocation: private std::span<std::byte> {
  public:
    static auto allocate(std::size_t size) -> MemoryAllocation;
    static auto deallocate(MemoryAllocation&&) noexcept -> void;

    auto view() const noexcept -> std::span<std::byte>;

    MemoryAllocation() = default;

  private:
    MemoryAllocation(std::byte* ptr, std::size_t size) :
        std::span<std::byte> {ptr, size} {}

    auto protect() -> void;
};

extern std::size_t page_size;

} // namespace concord::os