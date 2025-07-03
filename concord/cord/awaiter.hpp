#pragma once

#include <functional>

#include "concord/cord/handle.hpp"

namespace concord::cord {

class Awaiter: private std::move_only_function<CordHandle(CordHandle)> {
    using Base = std::move_only_function<CordHandle(CordHandle)>;

  public:
    using Base::operator();

    Awaiter() = default;

    template<typename F> // NOLINTNEXTLINE(google-explicit-constructor)
    Awaiter(F&& func)
        requires std::is_invocable_r_v<CordHandle, F, CordHandle>
        : Base(std::forward<F>(func)) {}

    template<typename F> // NOLINTNEXTLINE(google-explicit-constructor)
    Awaiter(F&& func)
        requires std::is_invocable_r_v<void, F, CordHandle>
        :
        Base([f = std::forward<F>(func)](CordHandle handle) {
            f(handle);
            return CordHandle::invalid();
        }) {}
};

} // namespace concord::cord