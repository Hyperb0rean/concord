#include <axis/monad/result.hpp>
#include <axis/monad/maybe.hpp>

#include "fmt/core.h"

using axis::monad::AndThen;
using axis::monad::Err;
using axis::monad::Error;
using axis::monad::Just;
using axis::monad::Maybe;
using axis::monad::Nothing;
using axis::monad::Ok;
using axis::monad::OrElse;
using axis::monad::Result;

auto TestResult() -> Result<int> {
  return Err(std::make_error_code(std::errc::invalid_argument));
}

auto TestMaybe() -> Maybe<int> {
  return Nothing();
}

auto main() -> int {
  TestResult()  //
      | OrElse([](Error e) {
          fmt::println("{}", e.message());
          return Ok(42);
        })  //
      | AndThen([](int v) {
          fmt::println("{}", v);
          return Ok(v);
        });

  TestMaybe()  //
      | OrElse([] {
          fmt::println("Nothing");
          return Just(42);
        })  //
      | AndThen([](int v) {
          fmt::println("{}", v);
          return Just(v);
        });
}