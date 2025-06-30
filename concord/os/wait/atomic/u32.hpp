#pragma once

#include <atomic>
#include <cstdint>

namespace concord::os::atomic {

class AtomicRefUint32 {
  public:
    using Type = uint32_t;

    explicit AtomicRefUint32(std::atomic<uint32_t>& atomic) :
        _atomic(&atomic) {}

    auto load(std::memory_order mo) const noexcept -> uint32_t {
        return _atomic->load(mo);
    }

    static auto value(uint32_t atomic_value) noexcept -> uint32_t {
        return atomic_value;
    }

    auto get() const noexcept -> uint32_t* {
        return reinterpret_cast<uint32_t*>(_atomic);
    }

  private:
    std::atomic<uint32_t>* _atomic;
};

} // namespace concord::os::atomic