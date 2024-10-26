

#include "mmap.hpp"

#ifdef LINUX
    #include "platform/posix.hpp"
#elif APPLE
    #include "platform/posix.hpp"
#elif WIN32
    #include "platform/windows.hpp"
#endif

namespace concord::syscall {

auto MemoryAllocation::allocate() -> char* {
    return concord::syscall::detail::mmap(
        _start,
        _size,
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE
    );
}

auto MemoryAllocation::protect() -> void {
    // Change to PAGE_SIZE
    concord::syscall::detail::mprotect(_start, 4096, PROT_NONE);
}

auto MemoryAllocation::deallocate() noexcept -> void {
    concord::syscall::detail::munmap(_start, _size);
}

} // namespace concord::syscall