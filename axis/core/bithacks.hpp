#pragma once

#include <cstddef>
#include <string>

// https://graphics.stanford.edu/~seander/bithacks.html

inline consteval auto GetBit(size_t x, size_t index) -> size_t {
  return (x >> index) & 1;
}

inline consteval auto IsPowerOfTwo(size_t x) -> bool {
  return (x & (x - 1)) == 0;
}

inline consteval auto MostSignificantBit(size_t x) -> size_t {
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);
  return x - (x >> 1);
}

inline consteval auto NextPowerOfTwo(size_t x) -> size_t {
  if (IsPowerOfTwo(x)) {
    return x;
  } else {
    return MostSignificantBit(x) << 1;
  }
}
