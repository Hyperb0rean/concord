

#pragma once

#include <cstddef>
#ifdef WIN32
    #include "memoryapi.h"
#else
void* VirtualAlloc(void*, std::size_t, int, int); // NOLINT
bool VirtualFree(void*, std::size_t); // NOLINT
bool VirtualProtect(void*, std::size_t, int, int*); // NOLINT
#endif

namespace {

// TODO: make flags work

auto mmap(std::byte* addr, std::size_t length, int prot, int flags) -> char* {
    return static_cast<char*>(
        VirtualAlloc(static_cast<void*>(addr), length, prot, flags)
    );
}

auto munmap(std::byte* addr, std::size_t length) -> int {
    return static_cast<int>(VirtualFree(static_cast<void*>(addr), length));
}

auto mprotect(std::byte* addr, std::size_t len, int prot) -> int {
    return static_cast<int>(
        VirtualProtect(static_cast<void*>(addr), len, prot, nullptr)
    );
}
} // namespace