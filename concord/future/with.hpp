#pragma once

#include "concord/future/concepts.hpp"
#include "concord/future/thunk/with.hpp"

namespace concord::future {

template<typename Bind>
struct With {
    [[no_unique_address]] Bind state_bind;

    template<Thunk InputFuture>
    auto then(InputFuture&& f) && {
        return thunk::With {std::move(f), std::move(state_bind)};
    }
};

} // namespace concord::future