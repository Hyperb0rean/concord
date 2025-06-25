#pragma once

#include <cassert>

#include "concord/syscall/mmap/mmap.hpp"

namespace concord::cord {

using Stack = std::span<std::byte>;

class StackAllocator {
    // TODO: Make Stack pool
  public:
    static auto allocate(std::size_t size, std::size_t align = 1)
        -> syscall::MemoryAllocation {
        assert(size % syscall::page_size == 0);
        const std::size_t total_size = size + syscall::page_size;
        return syscall::MemoryAllocation::allocate(total_size, align);
    }

    static auto dellocate(syscall::MemoryAllocation allocation) -> void {
        syscall::MemoryAllocation::deallocate(std::move(allocation));
    }
};

} // namespace concord::cord