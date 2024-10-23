#pragma once
#include <fmt/core.h>

#include <axis/core/defer.hpp>

inline auto print_hello() -> void {
    AXIS_DEFER {
        fmt::println("Defered!");
    };
    fmt::println("Hello!");
}