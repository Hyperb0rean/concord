

#include "mmap.hpp"

#ifdef LINUX
    #include "platform/posix.hpp"
#elif APPLE
    #include "platform/posix.hpp"
#elif WIN32
    #include "platform/windows.hpp"
#endif

namespace concord::os {

std::size_t page_size = system_page_size;

auto MemoryAllocation::allocate(std::size_t size) -> MemoryAllocation {
    auto* ptr = mmap(
        nullptr,
        size,
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE
    );
    MemoryAllocation allocation = {ptr, size};
    allocation.protect();
    return allocation;
}

auto MemoryAllocation::protect() -> void {
    mprotect(data(), page_size, PROT_NONE);
}

auto MemoryAllocation::deallocate(MemoryAllocation&& allocation) noexcept
    -> void {
    munmap(allocation.data(), allocation.size());
}

auto MemoryAllocation::view() const noexcept -> std::span<std::byte> {
    return {data(), size()};
}

} // namespace concord::os