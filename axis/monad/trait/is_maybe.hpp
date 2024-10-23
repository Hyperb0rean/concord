#pragma once

#include <axis/monad/maybe/type.hpp>
#include <type_traits>

namespace axis::monad {
namespace trait {

    template<typename T>
    struct IsMaybe {
        static constexpr bool value = false;
        using Type = std::false_type;
    };

    template<typename T>
    struct IsMaybe<Maybe<T>> {
        static constexpr bool value = true;
        using Type = std::true_type;
    };
} // namespace trait

template<typename T>
using IsMaybeType = typename trait::IsMaybe<T>::Type;

template<typename T>
concept IsMaybe = trait::IsMaybe<T>::value;

} // namespace axis::monad