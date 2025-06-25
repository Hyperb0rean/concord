#pragma once

#include <cassert>

#include "concord/os/mmap/mmap.hpp"

namespace concord::cord {

using Stack = std::span<std::byte>;

class StackAllocator {
    // TODO: Make Stack pool
  public:
    static auto allocate(std::size_t size, std::size_t align = 1)
        -> os::MemoryAllocation {
        assert(size % os::page_size == 0);
        const std::size_t total_size = size + os::page_size;
        return os::MemoryAllocation::allocate(total_size, align);
    }

    static auto dellocate(os::MemoryAllocation allocation) -> void {
        os::MemoryAllocation::deallocate(std::move(allocation));
    }
};

} // namespace concord::cord