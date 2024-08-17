#pragma once

#include "../type.hpp"
#include "../trait.hpp"

#include <type_traits>  // std::invoke_result_t

namespace axis::monad {

namespace combinator {

template <typename F>
struct [[nodiscard]] AndThen {
  F user;

  explicit AndThen(F u)
      : user(std::move(u)) {
  }

  // Non-copyable
  AndThen(const AndThen&) = delete;
  auto operator=(const AndThen&) -> AndThen& = delete;

  template <typename T>
  using U = ValueOf<std::invoke_result_t<F, T>>;

  template <typename T>
  auto Pipe(Result<T> r) -> Result<U<T>> {
    return std::move(r).and_then(user);
  }
};

}  // namespace combinator

/*
 * Result<T> -> (T -> Result<U>) -> Result<U>
 *
 * Usage:
 *
 * auto r = Ok(1) | AndThen([](int v) {
 *   return Ok(v + 1);
 * });
 *
 */

template <typename F>
auto AndThen(F user) {
  return combinator::AndThen{std::move(user)};
}

}  // namespace axis::monad
