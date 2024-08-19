#pragma once

#include <axis/monad/result/type.hpp>
#include <axis/monad/maybe/type.hpp>

namespace axis::monad {
namespace trait {
template <typename T>
struct ValueOf;

template <typename T>
struct ValueOf<Result<T>> {
  using Type = T;
};

template <typename T>
struct ValueOf<Maybe<T>> {
  using Type = T;
};

}  // namespace trait

template <typename Monad>
using ValueOf = typename trait::ValueOf<Monad>::Type;

}  // namespace axis::monad