#pragma once

#include <tuple>

#include "concord/future/concepts.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::future {

struct Runtime {
    rt::IRuntime* runtime;

    template<typename... Ss>
    auto operator()(std::tuple<Ss...> state)
        -> std::tuple<rt::IRuntime*, Ss...> {
        return [r = runtime]<std::size_t... Idx>(
                   std::tuple<Ss...>&& state,
                   std::index_sequence<sizeof...(Ss)>
               ) {
            return std::make_tuple(r, std::get<Idx>(state)...);
        }(std::move(state), std::make_index_sequence<sizeof...(Ss)> {});
    }

    template<typename S>
    auto operator()(S&& state) -> std::tuple<rt::IRuntime*, S> {
        return std::make_tuple(runtime, std::forward<S>(state));
    }

    auto operator()(Unit) -> rt::IRuntime* {
        return runtime;
    }
};

} // namespace concord::future