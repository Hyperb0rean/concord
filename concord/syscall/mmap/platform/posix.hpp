#pragma once

#include <sys/mman.h>
#include <unistd.h>

#include <cstddef>
#include <system_error>

namespace {
inline auto mmap(std::byte* addr, std::size_t length, int prot, int flags)
    -> std::byte* {
    auto* res = ::mmap(static_cast<void*>(addr), length, prot, flags, -1, 0);
    if (res == MAP_FAILED) {
        throw std::system_error {errno, std::generic_category()};
    }
    return static_cast<std::byte*>(res);
}

inline auto munmap(std::byte* addr, std::size_t length) noexcept -> int {
    // Assume never fails due to arguments always correct
    return ::munmap(static_cast<void*>(addr), length);
}

inline auto mprotect(std::byte* addr, std::size_t len, int prot) -> int {
    auto res = ::mprotect(static_cast<void*>(addr), len, prot);
    if (res == -1) {
        throw std::system_error {errno, std::generic_category()};
    }
    return res;
}

static constexpr std::size_t system_page_size = _SC_PAGESIZE;

} // namespace