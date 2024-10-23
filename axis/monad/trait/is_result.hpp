#pragma once

#include <axis/monad/result/type.hpp>
#include <type_traits>

namespace axis::monad {
namespace trait {

    template<typename T>
    struct IsResult {
        static constexpr bool value = false;
        using Type = std::false_type;
    };

    template<typename T>
    struct IsResult<Result<T>> {
        static constexpr bool value = true;
        using Type = std::true_type;
    };
} // namespace trait

template<typename T>
using IsResultType = typename trait::IsResult<T>::Type;

template<typename T>
concept IsResult = trait::IsResult<T>::value;

} // namespace axis::monad