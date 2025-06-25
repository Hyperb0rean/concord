#pragma once

#include <cstddef>
#include <span>

namespace concord::syscall {

class MemoryAllocation: public std::span<std::byte> {
  public:
    explicit MemoryAllocation(std::size_t size) :
        std::span<std::byte> {allocate(size), size} {
        protect();
    }

    auto view() const noexcept -> std::span<std::byte> {
        return static_cast<std::span<std::byte> const&>(*this);
    }

    ~MemoryAllocation() {
        deallocate();
    }

  private:
    static auto allocate(std::size_t size) -> std::byte*;
    auto protect() -> void;
    auto deallocate() noexcept -> void;
};

extern std::size_t page_size;

} // namespace concord::syscall