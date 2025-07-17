#pragma once

#include "concord/future/concepts.hpp"
#include "concord/future/thunk/map.hpp"

namespace concord::future {

template<typename F>
struct Map {
    [[no_unique_address]] F map;

    template<Thunk InputFuture>
    auto then(InputFuture&& f) && {
        return thunk::Map {std::move(f), std::move(map)};
    }
};

} // namespace concord::future