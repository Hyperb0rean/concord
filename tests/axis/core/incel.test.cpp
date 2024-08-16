#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <axis/core/incel.hpp>

using axis::InitializationCell;

struct Empty {};

struct Tricky {
  Tricky() = delete;

  Tricky(const Tricky&) = delete;
  Tricky(Tricky&&) = default;
  auto operator=(const Tricky&) -> Tricky& = delete;
  auto operator=(Tricky&&) -> Tricky& = delete;

  Tricky(int res)  // NOLINT
      : int_resource_(res),
        resource_(std::to_string(res)) {
  }
  operator int() {  // NOLINT
    return int_resource_;
  }

 private:
  int int_resource_;
  std::string resource_;
};

TEST(InitializationCell, Empty) {
  InitializationCell<Empty> cell;
  cell.Emplace(Empty{});
}

TEST(InitializationCell, Integer) {
  InitializationCell<int> cell;
  cell.Emplace(10);
  ASSERT_EQ(std::move(cell).Get(), 10);
}

TEST(InitializationCell, String) {
  InitializationCell<std::string> cell;
  cell.Emplace("Hello, world!");
  ASSERT_EQ(std::move(cell).Get(), "Hello, world!");
}

TEST(InitializationCell, Tricky) {
  InitializationCell<Tricky> cell;
  cell.Emplace(10);
  ASSERT_EQ(static_cast<int>(std::move(cell).Get()), 10);
}

TEST(InitializationCell, Move) {
  InitializationCell<Tricky> cell;
  cell.Emplace(100);
  InitializationCell<Tricky> cell2 = std::move(cell);
  cell.Emplace(200);
  ASSERT_EQ(static_cast<int>(std::move(cell2).Get()), 100);
  ASSERT_EQ(static_cast<int>(std::move(cell).Get()), 200);
}