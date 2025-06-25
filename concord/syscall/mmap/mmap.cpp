

#include "mmap.hpp"

#ifdef LINUX
    #include "platform/posix.hpp"
#elif APPLE
    #include "platform/posix.hpp"
#elif WIN32
    #include "platform/windows.hpp"
#endif

namespace concord::syscall {

std::size_t page_size = system_page_size;

auto MemoryAllocation::allocate(std::size_t size) -> std::byte* {
    return mmap(
        nullptr,
        size,
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE
    );
}

auto MemoryAllocation::protect() -> void {
    mprotect(data(), page_size, PROT_NONE);
}

auto MemoryAllocation::deallocate() noexcept -> void {
    munmap(data(), size());
}

} // namespace concord::syscall