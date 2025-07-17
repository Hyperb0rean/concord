#pragma once

#include <concepts>
#include <utility>

namespace concord::future {

struct Unit {};

template<typename C>
concept Computation = requires(C comp) {
    { comp.call() } -> std::same_as<void>;
};

template<typename C, typename V, typename S>
concept Continuation = requires(C cont, V v, S st) {
    { cont.resume(std::move(v), std::move(st)) } -> std::same_as<void>;
};

template<typename V, typename S>
struct Demand {
    void resume(V, S) {}
};

template<typename T>
concept Thunk =
    requires(T thunk, Demand<typename T::ValueType, typename T::State> demand) {
        typename T::ValueType;
        typename T::State;

        { thunk.materialize(std::move(demand)) } -> Computation;
    };

template<Thunk T>
using value_of = T::ValueType; // NOLINT(readability-identifier-naming)

template<Thunk T>
using state_of = T::State; // NOLINT(readability-identifier-naming)

template<typename F, typename V, typename S>
concept Future =
    Thunk<F> and std::same_as<value_of<F>, V> and std::same_as<state_of<F>, S>;

template<Thunk Future, Continuation<value_of<Future>, state_of<Future>> Demand>
using Materialize =
    decltype(std::declval<Future>().materialize(std::declval<Demand>()));

} // namespace concord::future