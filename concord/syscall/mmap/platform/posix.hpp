#pragma once

#include <sys/mman.h>

#include <cstddef>

namespace concord::syscall::detail {
auto mmap(char* addr, std::size_t length, int prot, int flags) -> char* {
    return reinterpret_cast<char*>(
        ::mmap(reinterpret_cast<void*>(addr), length, prot, flags, -1, 0)
    );
}

auto munmap(char* addr, std::size_t length) -> int {
    return ::munmap(reinterpret_cast<void*>(addr), length);
}

auto mprotect(char* addr, std::size_t len, int prot) -> int {
    return ::mprotect(reinterpret_cast<void*>(addr), len, prot);
}
} // namespace concord::syscall::detail