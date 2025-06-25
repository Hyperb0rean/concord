#pragma once

#include "fwd.hpp"

namespace concord::cord {
class CordHandle {
    friend Cord;

  public:
    CordHandle() = default;

    static auto invalid() -> CordHandle {
        return {};
    }

    auto is_valid() const -> bool {
        return _cord != nullptr;
    }

    auto spawn() -> void;

    auto switch_to() -> void;

    auto get() -> Cord*;

  private:
    explicit CordHandle(Cord* cord) : _cord(cord) {}

    auto release() -> Cord*;

  private:
    Cord* _cord {nullptr};
};
} // namespace concord::cord