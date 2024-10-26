

#pragma once

#include <cstddef>
#ifdef WIN32
    #include "memoryapi.h"
#else
void* VirtualAlloc(void*, std::size_t, int, int); // NOLINT
bool VirtualFree(void*, std::size_t); // NOLINT
bool VirtualProtect(void*, std::size_t, int, int*); // NOLINT
#endif

namespace concord::syscall::detail {

// TODO: make flags work

auto mmap(char* addr, std::size_t length, int prot, int flags) -> char* {
    return reinterpret_cast<char*>(
        VirtualAlloc(reinterpret_cast<void*>(addr), length, prot, flags)
    );
}

auto munmap(char* addr, std::size_t length) -> int {
    return static_cast<int>(VirtualFree(reinterpret_cast<void*>(addr), length));
}

auto mprotect(char* addr, std::size_t len, int prot) -> int {
    return static_cast<int>(
        VirtualProtect(reinterpret_cast<void*>(addr), len, prot, nullptr)
    );
}
} // namespace concord::syscall::detail