#pragma once

#include <atomic>
#include <cstdint>

namespace concord::os::sync {

class Mutex {
  public:
    auto lock() -> void;
    auto unlock() -> void;

  private:
    struct State {
        enum _ : std::uint32_t { Free = 0, Locked = 1, Awaited = 2 };
    };

    std::atomic<uint32_t> _state {State::Free};
};

} // namespace concord::os::sync