#pragma once
#include <functional>
#include "preprocessor.hpp"

namespace axis {
template <typename F>
class Defer {
 public:
  // NB: no explicit see https://github.com/facebook/folly/blob/main/folly/ScopeGuard.h
  // NOLINTNEXTLINE
  Defer(F&& f) : func_(std::forward<F>(f)) {
  }

  ~Defer() {
    func_();
  }

 private:
  F func_;
};

}

#define AXIS_DEFER \
    [[maybe_unused]] ::axis::Defer AXIS_ANONYMOUS(axis_defer) = [&]() noexcept

