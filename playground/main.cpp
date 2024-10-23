#include <axis/monad/maybe.hpp>
#include <axis/monad/result.hpp>

#include "fmt/core.h"

using axis::monad::and_then;
using axis::monad::Err;
using axis::monad::Error;
using axis::monad::Just;
using axis::monad::Maybe;
using axis::monad::None;
using axis::monad::Ok;
using axis::monad::or_else;
using axis::monad::Result;

auto main() -> int {
    Maybe<int>(None()) //
        | or_else([] {
              fmt::println("Nothing");
              return Just(42);
          }) //
        | and_then([](int v) {
              fmt::println("{}", v);
              return Just(v);
          });
}