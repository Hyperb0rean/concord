#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <span>

namespace concord::syscall {

class MemoryAllocation: private std::span<std::byte> {
  public:
    MemoryAllocation() = default;

    explicit MemoryAllocation(std::size_t size, std::size_t align) :
        std::span<std::byte> {allocate(size), size},
        _align {align} {
        protect();
    }

    auto view() const noexcept -> std::span<std::byte> {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(&back());
        uintptr_t aligned = ptr & ~(_align - 1);
        assert(aligned <= ptr);
        uintptr_t diff = ptr - aligned;
        return {begin(), size() - diff};
    }

    ~MemoryAllocation() {
        deallocate();
    }

  private:
    static auto allocate(std::size_t size) -> std::byte*;
    auto protect() -> void;
    auto deallocate() noexcept -> void;

    std::size_t _align;
};

extern std::size_t page_size;

} // namespace concord::syscall