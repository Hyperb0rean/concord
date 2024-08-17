#pragma once

#include "../type.hpp"

#include <type_traits>  // std::invoke_result_t

namespace axis::monad {

namespace combinator {

template <typename F>
struct [[nodiscard]] Map {
  F user;

  explicit Map(F u)
      : user(std::move(u)) {
  }

  // Non-copyable
  Map(const Map&) = delete;
  auto operator=(const Map&) -> Map& = delete;

  template <typename T>
  using U = std::invoke_result_t<F, T>;

  template <typename T>
  auto Pipe(Result<T> r) -> Result<U<T>> {
    return std::move(r).transform(user);
  }
};

}  // namespace combinator

/*
 * Result<T> -> (T -> U) -> Result<U>
 *
 * Usage:
 *
 * auto r = Ok(1) | Map([](int v) {
 *   return std::to_string(v);
 * });
 *
 */

template <typename F>
auto Map(F user) {
  return combinator::Map{std::move(user)};
}

}  // namespace axis::monad
