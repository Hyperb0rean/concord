#pragma once

#include <__bit/endian.h>

#include <atomic>
#include <cstdint>

#include "axis/core/bithacks.hpp"

namespace concord::syscall::atomic {

enum class AtomicHalfUint64 {
    Low = 0,
    High,
};

template<AtomicHalfUint64 Half>
class AtomicRefUint64 {
  public:
    using Type = uint64_t;

    explicit AtomicRefUint64(std::atomic<uint64_t>& atomic) :
        _atomic(&atomic) {}

    auto load(std::memory_order mo) const noexcept -> uint64_t {
        return _atomic->load(mo);
    }

    static auto value(uint64_t atomic_value) noexcept -> uint32_t {
        if constexpr (is_low_half()) {
            return static_cast<uint32_t>(axis::get_low_half(atomic_value));
        } else {
            return static_cast<uint32_t>(axis::get_high_half(atomic_value));
        }
    }

    auto location() const noexcept -> uint32_t* {
        if constexpr (is_low_half()) {
            return low_half_location();
        } else {
            return high_half_location();
        };
    }

  private:
    auto pointer() const noexcept -> uint32_t* {
        return reinterpret_cast<uint32_t*>(_atomic);
    }

    auto low_half_location() const noexcept -> uint32_t* {
        if constexpr (is_little_endian()) {
            return pointer() + 0;
        } else if constexpr (is_big_endian()) {
            return pointer() + 1;
        } else {
            std::abort(); // Not supported
        }
    }

    auto high_half_location() const noexcept -> uint32_t* {
        if constexpr (is_little_endian()) {
            return pointer() + 1;
        } else if constexpr (is_big_endian()) {
            return pointer() + 0;
        } else {
            std::abort(); // Not supported
        }
    }

    static constexpr auto is_low_half() -> bool {
        return Half == AtomicHalfUint64::Low;
    }

    static constexpr auto is_little_endian() -> bool {
        return std::endian::native == std::endian::little;
    }

    static constexpr auto is_big_endian() -> bool {
        return std::endian::native == std::endian::big;
    }

  private:
    std::atomic<uint64_t>* _atomic;
};

} // namespace concord::syscall::atomic