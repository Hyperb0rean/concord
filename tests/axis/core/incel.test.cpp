#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <axis/core/incel.hpp>

using axis::InitializationCell;

struct Empty {};

struct Tricky {
    Tricky() = delete;

    Tricky(const Tricky&) = delete;
    Tricky(Tricky&&) = default;
    auto operator=(const Tricky&) -> Tricky& = delete;
    auto operator=(Tricky&&) -> Tricky& = delete;

    Tricky(int res) // NOLINT
        :
        _int_resource(res),
        _resource(std::to_string(res)) {}

    operator int() { // NOLINT
        return _int_resource;
    }

  private:
    int _int_resource;
    std::string _resource;
};

TEST(InitializationCell, Empty) {
    InitializationCell<Empty> cell;
    cell.emplace(Empty {});
}

TEST(InitializationCell, Integer) {
    InitializationCell<int> cell;
    cell.emplace(10);
    ASSERT_EQ(std::move(cell).get(), 10);
}

TEST(InitializationCell, String) {
    InitializationCell<std::string> cell;
    cell.emplace("Hello, world!");
    ASSERT_EQ(std::move(cell).get(), "Hello, world!");
}

TEST(InitializationCell, Tricky) {
    InitializationCell<Tricky> cell;
    cell.emplace(10);
    ASSERT_EQ(static_cast<int>(std::move(cell).get()), 10);
}

TEST(InitializationCell, Move) {
    InitializationCell<Tricky> cell;
    cell.emplace(100);
    InitializationCell<Tricky> cell2 = std::move(cell);
    cell.emplace(200);
    ASSERT_EQ(static_cast<int>(std::move(cell2).get()), 100);
    ASSERT_EQ(static_cast<int>(std::move(cell).get()), 200);
}