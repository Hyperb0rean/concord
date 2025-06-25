#pragma once

namespace concord::cord {

struct IRunnable {
    virtual auto run [[noreturn]] () noexcept -> void = 0;

  protected:
    ~IRunnable() = default;
};

} // namespace concord::cord