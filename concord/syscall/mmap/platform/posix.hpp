#pragma once

#include <sys/mman.h>

#include <cstddef>

namespace concord::syscall::detail {
auto mmap(char* addr, std::size_t length, int prot, int flags) -> char* {
    return static_cast<char*>(
        ::mmap(static_cast<void*>(addr), length, prot, flags, -1, 0)
    );
}

auto munmap(char* addr, std::size_t length) -> int {
    return ::munmap(static_cast<void*>(addr), length);
}

auto mprotect(char* addr, std::size_t len, int prot) -> int {
    return ::mprotect(static_cast<void*>(addr), len, prot);
}
} // namespace concord::syscall::detail