#pragma once

#include "concord/cord/handle.hpp"

namespace concord::cord {

struct IAwaiter {
    virtual auto await(CordHandle) -> CordHandle = 0;
};

} // namespace concord::cord