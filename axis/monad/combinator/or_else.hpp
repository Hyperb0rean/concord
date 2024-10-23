#pragma once

#include <axis/monad/result/type.hpp>
#include <axis/monad/trait/is_maybe.hpp>
#include <axis/monad/trait/is_result.hpp>
#include <axis/monad/trait/value_of.hpp>

namespace axis::monad {

namespace combinator {

    template<typename F>
    struct [[nodiscard]] OrElse {
        F user;

        explicit OrElse(F u) : user(std::move(u)) {}

        // Non-copyable
        OrElse(const OrElse&) = delete;
        auto operator=(const OrElse&) -> OrElse& = delete;

        template<typename Monad>
        requires IsResult<Monad>

        auto pipe(Monad r) -> Result<ValueOf<Monad>> {
            return std::move(r).or_else(user);
        }

        template<typename Monad>
        requires IsMaybe<Monad>

        auto pipe(Monad r) -> Maybe<ValueOf<Monad>> {
            return std::move(r).or_else(user);
        }
    };

} // namespace combinator

/*
 * Monad<T> -> (Error | Nothing -> Monad<T>) -> Monad<T>
 *
 * Usage:
 *
 * auto failure = [] -> Result<int> {
 *   return Err(TimeoutError());
 * }
 *
 * auto r = failure() | result::OrElse([](Error) {
 *   return Ok(42);  // Fallback
 * });
 *
 */

template<typename F>
auto or_else(F user) {
    return combinator::OrElse {std::move(user)};
}

} // namespace axis::monad
