#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <axis/core/bithacks.hpp>

TEST(InitializationCell, GetBit) {
  const size_t check = 0x0FFFFFFF;
  ASSERT_EQ(GetBit(check, 0), 1);
  ASSERT_EQ(GetBit(check, 63), 0);
  ASSERT_EQ(GetBit(check, 14), 1);
}