#pragma once

#include <axis/monad/result/type.hpp>
#include <axis/monad/trait/is_maybe.hpp>
#include <axis/monad/trait/is_result.hpp>
#include <axis/monad/trait/value_of.hpp>
#include <type_traits> // std::invoke_result_t

namespace axis::monad {

namespace combinator {

    template<typename F>
    struct [[nodiscard]] AndThen {
        F user;

        explicit AndThen(F u) : user(std::move(u)) {}

        // Non-copyable
        AndThen(const AndThen&) = delete;
        auto operator=(const AndThen&) -> AndThen& = delete;

        template<typename T>
        using U = ValueOf<std::invoke_result_t<F, T>>;

        template<typename Monad>
        requires IsResult<Monad>

        auto pipe(Monad r) -> Result<U<ValueOf<Monad>>> {
            return std::move(r).and_then(user);
        }

        template<typename Monad>
        requires IsMaybe<Monad>

        auto pipe(Monad r) -> Maybe<U<ValueOf<Monad>>> {
            return std::move(r).and_then(user);
        }
    };

} // namespace combinator

/*
 * Monad<T> -> (T -> Monad<U>) -> Monad<U>
 *
 * Usage:
 *
 * auto r = Ok(1) | AndThen([](int v) {
 *   return Ok(v + 1);
 * });
 *
 */

template<typename F>
auto and_then(F user) {
    return combinator::AndThen {std::move(user)};
}

} // namespace axis::monad
