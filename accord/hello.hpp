#pragma once
#include <fmt/core.h>
#include <axis/core/defer.hpp>

inline auto PrintHello() -> void {
  AXIS_DEFER {
    fmt::println("Defered!");
  };
  fmt::println("Hello!");
}