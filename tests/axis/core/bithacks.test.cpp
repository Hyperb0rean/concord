#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <axis/core/bithacks.hpp>

TEST(Bithacks, GetBit) {
  EXPECT_EQ(GetBit(0x0FFFFFFF, 0), 1);
  EXPECT_EQ(GetBit(0x0FFFFFFF, 63), 0);
  EXPECT_EQ(GetBit(0x0FFFFFFF, 14), 1);
  EXPECT_EQ(GetBit(0x0FFFFFFF, 32), 0);
}

TEST(Bithacks, IsPowerOfTwo) {
  EXPECT_TRUE(IsPowerOfTwo(2));
  EXPECT_TRUE(IsPowerOfTwo(1024));
  EXPECT_FALSE(IsPowerOfTwo(3));
  EXPECT_FALSE(IsPowerOfTwo(15));

  // NB
  EXPECT_TRUE(IsPowerOfTwo(0));
}

TEST(Bithacks, NextPowerOfTwo) {
  EXPECT_EQ(RoundPowerOfTwo(2), 2);
  EXPECT_EQ(RoundPowerOfTwo(1024), 1024);
  EXPECT_EQ(RoundPowerOfTwo(3), 4);
  EXPECT_EQ(RoundPowerOfTwo(1337), 2048);

  // NB
  EXPECT_EQ(RoundPowerOfTwo(0), 0);
}

TEST(Bithacks, BinaryLog) {
  EXPECT_EQ(BinaryLog(16), 4);
  EXPECT_EQ(BinaryLog(1024), 10);
  EXPECT_EQ(BinaryLog(1024 * 1024 + 1024), 20);
  EXPECT_EQ(BinaryLog(1337), 10);
}

TEST(Bithacks, GetByte) {
  EXPECT_EQ(GetByte(0x1234567890ABCDEF, 0), 0xEF);
  EXPECT_EQ(GetByte(0x1234567890ABCDEF, 1), 0xCD);
  EXPECT_EQ(GetByte(0x1234567890ABCDEF, 2), 0xAB);
  EXPECT_EQ(GetByte(0x1234567890ABCDEF, 3), 0x90);
}

TEST(Bithacks, ReverseByte) {
  EXPECT_EQ(ReverseByte(0x00), 0x00);
  EXPECT_EQ(ReverseByte(0x01), 0x80);
  EXPECT_EQ(ReverseByte(0x80), 0x01);
  EXPECT_EQ(ReverseByte(0xFF), 0xFF);
}

TEST(ByteOperationsTest, ReverseBits) {
  EXPECT_EQ(ReverseBits(0x1234567890ABCDEF), 0xF7B3D5091E6A2C48);
  EXPECT_EQ(ReverseBits(0x0123456789ABCDEF), 0xF7B3D591E6A2C480);
  EXPECT_EQ(ReverseBits(0x00000000FFFFFFFF), 0xFFFFFFFF00000000);
  EXPECT_EQ(ReverseBits(0xFFFFFFFF00000000), 0x00000000FFFFFFFF);
}

TEST(ByteOperationsTest, NextLexPermutation) {
  EXPECT_EQ(NextLexPermutation(0x0), 0x7FFFFFFF);
}

TEST(Bithacks, GetLowHalf) {
  EXPECT_EQ(GetLowHalf(0x1234567890ABCDEF), 0x90ABCDEF);
  EXPECT_EQ(GetLowHalf(0x0000000012345678), 0x12345678);
  EXPECT_EQ(GetLowHalf(0xFFFFFFFF00000000), 0x00000000);
}

TEST(Bithacks, GetHighHalf) {
  EXPECT_EQ(GetHighHalf(0x1234567890ABCDEF), 0x12345678);
  EXPECT_EQ(GetHighHalf(0x0000000012345678), 0x0000000);
  EXPECT_EQ(GetHighHalf(0xFFFFFFFF00000000), 0xFFFFFFFF);
}
