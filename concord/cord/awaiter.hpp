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
        requires requires(F f, CordHandle h) {
            { f(h) } -> std::same_as<CordHandle>;
        }
        : Base(std::forward<F>(func)) {}

    template<typename F> // NOLINTNEXTLINE(google-explicit-constructor)
    Awaiter(F&& func)
        requires requires(F f, CordHandle h) {
            { f(h) } -> std::same_as<void>;
        }
        :
        Base([f = std::forward<F>(func)](CordHandle handle) {
            f(handle);
            return CordHandle::invalid();
        }) {}

    template<typename F> // NOLINTNEXTLINE(google-explicit-constructor)
    Awaiter(F&& func)
        requires requires(F f, CordHandle h) {
            { f(h) } -> std::same_as<bool>;
        }
        :
        Base([f = std::forward<F>(func)](CordHandle handle) {
            if (f(handle)) {
                return handle;
            }
            return CordHandle::invalid();
        }) {}
};

} // namespace concord::cord