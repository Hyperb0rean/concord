#pragma once
#include <atomic>

#include "fmt/base.h"

namespace {
auto wait(uint32_t* addr, uint32_t old) -> int;
}

namespace concord::os::atomic {
template<typename AtomicT>
auto wait(AtomicT atom_ref, uint32_t old, std::memory_order mo) ->
    typename AtomicT::Type {
    typename AtomicT::Type atomic_curr_value = 0;
    do {
        ::wait(atom_ref.location(), old);
        atomic_curr_value = atom_ref.load(mo);
    } while (AtomicT::value(atomic_curr_value) == old);

    return atomic_curr_value;
}

} // namespace concord::os::atomic
