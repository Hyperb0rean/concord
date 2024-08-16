#pragma once

#include <memory>
#include <utility>

namespace axis {

// Affine type
template <typename T>
class InitializationCell final {
 public:
  template <typename... Args>
  auto Emplace(Args&&... args) -> void {
    std::construct_at<T>(Pointer(), std::forward<Args>(args)...);
  }

 public:
  auto Get() && noexcept -> T {
    auto&& val = std::move(*Pointer());
    std::destroy_at(Pointer());
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
    Emplace(std::move(*other.Pointer()));
  };
  auto operator=(InitializationCell&& other) -> InitializationCell& {
    std::destroy_at(Pointer());
    Emplace(std::move(*other.Pointer()));
  };

 private:
  auto Pointer() noexcept -> T* {
    return std::launder(reinterpret_cast<T*>(std::addressof(storage_)));
  }

  alignas(T) std::byte storage_[sizeof(T)]{};
};
}  // namespace axis