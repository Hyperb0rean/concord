#pragma once

#include "../type.hpp"

namespace axis::monad {

namespace combinator {

template <typename F>
struct [[nodiscard]] OrElse {
  F user;

  explicit OrElse(F u)
      : user(std::move(u)) {
  }

  // Non-copyable
  OrElse(const OrElse&) = delete;
  auto operator=(const OrElse&) -> OrElse& = delete;

  template <typename T>
  auto Pipe(Result<T> r) -> Result<T> {
    return std::move(r).or_else(user);
  }
};

}  // namespace combinator

/*
 * Result<T> -> (Error -> Result<T>) -> Result<T>
 *
 * Usage:
 *
 * auto failure = [] -> Result<int> {
 *   return Err(TimeoutError());
 * }
 *
 * auto r = failure() | result::OrElse([](Error) {
 *   return Ok(42);  // Fallback
 * });
 *
 */

template <typename F>
auto OrElse(F user) {
  return combinator::OrElse{std::move(user)};
}

}  // namespace axis::monad
