#pragma once

#include <cstddef>

namespace concord::syscall::detail {

auto mmap(char* addr, std::size_t length, int prot, int flags) -> char*;
auto munmap(char* addr, std::size_t length) -> int;
auto mprotect(char* addr, std::size_t len, int prot) -> int;

} // namespace concord::syscall::detail