#pragma once

#include <memory>
#include <utility>

namespace axis {

// Affine type
template<typename T>
class InitializationCell final {
  public:
    template<typename... Args>
    auto emplace(Args&&... args) -> void {
        std::construct_at<T>(pointer(), std::forward<Args>(args)...);
    }

  public:
    auto get() && noexcept -> T {
        auto&& val = std::move(*pointer());
        std::destroy_at(pointer());
        return val;
    }

  public:
    ~InitializationCell() = default;
    InitializationCell() = default;

    // Non copyable
    InitializationCell(const InitializationCell&) = delete;
    auto operator=(const InitializationCell&) -> InitializationCell& = delete;

    // Moveable
    InitializationCell(InitializationCell&& other) {
        emplace(std::move(*other.pointer()));
    };

    auto operator=(InitializationCell&& other) -> InitializationCell& {
        std::destroy_at(pointer());
        Emplace(std::move(*other.pointer()));
    };

  private:
    auto pointer() noexcept -> T* {
        return reinterpret_cast<T*>(std::addressof(_storage));
    }

    alignas(T) std::byte _storage[sizeof(T)] {};
};
} // namespace axis