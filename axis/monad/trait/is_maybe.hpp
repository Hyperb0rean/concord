#pragma once

#include <type_traits>
#include <axis/monad/maybe/type.hpp>

namespace axis::monad {
namespace trait {

template <typename T>
struct IsMaybe {
  static constexpr bool kValue = false;
  using Type = std::false_type;
};

template <typename T>
struct IsMaybe<Maybe<T>> {
  static constexpr bool kValue = true;
  using Type = std::true_type;
};
}  // namespace trait

template <typename T>
using IsMaybeType = typename trait::IsMaybe<T>::Type;

template <typename T>
concept IsMaybe = trait::IsMaybe<T>::kValue;

}  // namespace axis::monad