#pragma once

#include "concord/cord/fwd.hpp"

namespace {
thread_local concord::cord::Cord* current = nullptr;
}

namespace concord::cord {

class Current {
  public:
    static auto get() -> Cord* {
        return current;
    }

    static auto set(Cord* ptr) -> void {
        current = ptr;
    }

  private:
};
} // namespace concord::cord