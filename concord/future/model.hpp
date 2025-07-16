#pragma once

#include <concepts>
#include <utility>

namespace concord::future {

template<typename C>
concept Computation = requires(C comp) {
    { comp.call() } -> std::same_as<void>;
};

template<typename C, typename V>
concept Continuation = requires(C cont, V v) {
    { cont.resume(std::move(v)) } -> std::same_as<void>;
};

template<typename V>
struct Demand {
    void resume(V) {}
};

template<typename T>
concept Thunk = requires(T thunk, Demand<typename T::ValueType> demand) {
    typename T::value_type;

    { thunk.materialize(std::move(demand)) } -> Computation;
};

template<Thunk T>
using ValueOf = T::value_type;

template<typename F, typename V>
concept Future = Thunk<F> and std::same_as<ValueOf<F>, V>;

template<Thunk Future, Continuation<ValueOf<Future>> Demand>
using Materialize =
    decltype(std::declval<Future>().materialize(std::declval<Demand>()));

} // namespace concord::future