#pragma once
#include <functional>
#include "preprocessor.hpp"

namespace axis {
namespace detail {
template <typename F>
class Defer {
 public:
  // NB: no explicit see
  // https://github.com/facebook/folly/blob/main/folly/ScopeGuard.h
  // NOLINTNEXTLINE
  Defer(F&& f)
      : func_(std::forward<F>(f)) {
  }

  // Non copyable
  Defer(const Defer&) = delete;
  auto operator=(const Defer&) -> Defer& = delete;

  // Non moveable
  Defer(Defer&&) noexcept = delete;
  auto operator=(Defer&&) noexcept -> Defer& = delete;

  ~Defer() {
    func_();
  }

 private:
  F func_;
};
}  // namespace detail

}  // namespace axis

#define AXIS_DEFER                                       \
  [[maybe_unused]] ::axis::detail::Defer AXIS_ANONYMOUS( \
      axis_defer) = [&]() noexcept
