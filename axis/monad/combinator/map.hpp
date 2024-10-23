#pragma once

#include <axis/monad/result/type.hpp>
#include <axis/monad/trait/is_maybe.hpp>
#include <axis/monad/trait/is_result.hpp>
#include <axis/monad/trait/value_of.hpp>
#include <type_traits> // std::invoke_result_t

namespace axis::monad {

namespace combinator {

    template<typename F>
    struct [[nodiscard]] Map {
        F user;

        explicit Map(F u) : user(std::move(u)) {}

        // Non-copyable
        Map(const Map&) = delete;
        auto operator=(const Map&) -> Map& = delete;

        template<typename T>
        using U = std::invoke_result_t<F, T>;

        template<typename Monad>
        requires IsResult<Monad>

        auto pipe(Monad r) -> Result<U<ValueOf<Monad>>> {
            return std::move(r).transform(user);
        }

        template<typename Monad>
        requires IsMaybe<Monad>

        auto pipe(Monad r) -> Maybe<U<ValueOf<Monad>>> {
            return std::move(r).transform(user);
        }
    };

} // namespace combinator

/*
 * Monad<T> -> (T -> U) -> Monad<U>
 *
 * Usage:
 *
 * auto r = Ok(1) | Map([](int v) {
 *   return std::to_string(v);
 * });
 *
 */

template<typename F>
auto map(F user) {
    return combinator::Map {std::move(user)};
}

} // namespace axis::monad
