#pragma once

#include <cstddef>
#include <cstdint>

#include "concord/syscall/mmap/platform/mmap.hpp"

namespace concord::syscall {

class MemoryAllocation final {
  public:
    MemoryAllocation(char* start, std::size_t size) :
        _start(start),
        _size(size) {
        allocate();
        protect();
    }

    ~MemoryAllocation() {
        deallocate();
    }

  private:
    auto allocate() -> char*;
    auto protect() -> void;
    auto deallocate() noexcept -> void;

    char* _start;
    std::size_t _size;
};

} // namespace concord::syscall