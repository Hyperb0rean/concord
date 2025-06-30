#pragma once

#include <atomic>

namespace concord::os::sync {

class WaitGroup {
  public:
    auto add(std::size_t count) -> void;

    auto done() -> void;

    auto wait() -> void;

  private:
    // high for work count / low for waiters count
    std::atomic<std::uint64_t> _state {0};
};
} // namespace concord::os::sync