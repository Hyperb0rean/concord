#include <axis/monad/result.hpp>
#include "fmt/core.h"

using axis::monad::AndThen;
using axis::monad::Err;
using axis::monad::Error;
using axis::monad::Ok;
using axis::monad::OrElse;
using axis::monad::Result;

auto Test() -> Result<int> {
  return Err(std::make_error_code(std::errc::invalid_argument));
}

auto main() -> int {
  Test() | OrElse([](Error e) {
    fmt::println("{}", e.message());
    return Ok(42);
  }) | AndThen([](int v) {
    fmt::println("{}", v);
    return Ok(v);
  });
}