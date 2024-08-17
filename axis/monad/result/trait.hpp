#pragma once

#include <type_traits>
#include "type.hpp"

namespace axis::monad {
namespace match {

template <typename T>
struct IsResult {
  static constexpr bool kValue = false;
  using Type = std::false_type;
};

template <typename T>
struct IsResult<Result<T>> {
  static constexpr bool kValue = true;
  using Type = std::true_type;
};

template <typename T>
struct ValueOf;

template <typename T>
struct ValueOf<Result<T>> {
  using Type = T;
};

}  // namespace match

template <typename T>
using IsResultType = typename match::IsResult<T>::Type;

template <typename T>
concept IsResult = match::IsResult<T>::kValue;

template <typename Result>
using ValueOf = typename match::ValueOf<Result>::Type;

}  // namespace axis::monad