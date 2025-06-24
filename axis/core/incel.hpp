#pragma once

#include <memory>
#include <utility>

namespace axis {

// Affine type
template<typename T>
class InitializationCell final {
  public:
    template<typename... Args>
    auto
    emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>
    ) -> void {
        std::construct_at<T>(pointer(), std::forward<Args>(args)...);
    }

  public:
    auto get() && noexcept(std::is_nothrow_move_constructible_v<T>) -> T {
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
    InitializationCell(
        InitializationCell&& other
    ) noexcept(std::is_nothrow_move_constructible_v<T>) {
        emplace(std::move(*other.pointer()));
    };

    auto operator=(InitializationCell&& other
    ) noexcept(std::is_nothrow_move_constructible_v<T>) -> InitializationCell& {
        std::destroy_at(pointer());
        emplace(std::move(*other.pointer()));
    };

  private:
    auto pointer() noexcept -> T* {
        return reinterpret_cast<T*>(std::addressof(_storage));
    }

    alignas(T) std::byte _storage[sizeof(T)] {};
};
} // namespace axis