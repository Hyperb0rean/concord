#pragma once

namespace concord::cord {

struct IRunnable {
    virtual auto run [[noreturn]] () noexcept -> void = 0;
    virtual ~IRunnable() = 0;
};

} // namespace concord::cord