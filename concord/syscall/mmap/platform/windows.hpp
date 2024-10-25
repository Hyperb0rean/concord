

#pragma once

#include <cstddef>
#ifdef WIN32
    #include "memoryapi.h"
#else
#endif

namespace concord::syscall::detail {
auto mmap(char* addr, std::size_t length, int prot, int flags) -> char* {
    return reinterpret_cast<char*>(
        VirtualAlloc(reinterpret_cast<void*>(addr), length, prot, flags, -1, 0)
    );
}

auto munmap(char* addr, std::size_t length) -> int {
    return VirtualFree(reinterpret_cast<void*>(addr), length);
}

auto mprotect(char* addr, std::size_t len, int prot) -> int {
    return VirtualProtect(reinterpret_cast<void*>(addr), len, prot);
}
} // namespace concord::syscall::detail