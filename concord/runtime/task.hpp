#pragma once

namespace concord::runtime {
struct ITask {
    virtual auto run() noexcept -> void = 0;

  protected:
    ~ITask() = default;
};
} // namespace concord::runtime