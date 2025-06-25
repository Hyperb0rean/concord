#pragma once

#include <cassert>

#include "concord/syscall/mmap/mmap.hpp"

namespace concord::cord {

using Stack = std::span<std::byte>;

class StackAllocator {
  public:
    static auto allocate(std::size_t size, std::size_t align = 1)
        -> syscall::MemoryAllocation {
        assert(size % syscall::page_size);
        const std::size_t total_size = size + syscall::page_size;
        return syscall::MemoryAllocation(total_size, align);
    }
};

} // namespace concord::cord