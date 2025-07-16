#pragma once

#include <atomic>
#include <cstdint>

namespace concord::os::sync {

class Event {
  public:
    auto wait() noexcept -> void;
    auto fire() noexcept -> void;

  private:
    struct State {
        enum _ : std::uint32_t { Initialized = 0, Fired = 1 };
    };

    std::atomic<uint32_t> _state {State::Initialized};
};

} // namespace concord::os::sync