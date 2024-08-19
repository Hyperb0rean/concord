#pragma once

#include <type_traits>
#include <axis/monad/result/type.hpp>

namespace axis::monad {
namespace trait {

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
}  // namespace trait

template <typename T>
using IsResultType = typename trait::IsResult<T>::Type;

template <typename T>
concept IsResult = trait::IsResult<T>::kValue;

}  // namespace axis::monad