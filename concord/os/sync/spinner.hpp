#pragma once

#include <cstddef>

namespace concord::os::sync {

template<std::size_t parking = 100>
struct Spinner {
    auto keep_spinning() const noexcept -> bool {
        return _spins >= parking;
    }

    auto operator()() noexcept -> void {
        ++_spins;
    }

  private:
    std::size_t _spins;
};

} // namespace concord::os::sync