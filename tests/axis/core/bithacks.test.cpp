#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <axis/core/bithacks.hpp>

using namespace axis; // NOLINT

TEST(Bithacks, get_bit) {
    EXPECT_EQ(get_bit(0x0FFFFFFF, 0), 1);
    EXPECT_EQ(get_bit(0x0FFFFFFF, 63), 0);
    EXPECT_EQ(get_bit(0x0FFFFFFF, 14), 1);
    EXPECT_EQ(get_bit(0x0FFFFFFF, 32), 0);
}

TEST(Bithacks, is_power_of_two) {
    EXPECT_TRUE(is_power_of_two(2));
    EXPECT_TRUE(is_power_of_two(1024));
    EXPECT_FALSE(is_power_of_two(3));
    EXPECT_FALSE(is_power_of_two(15));

    // NB
    EXPECT_TRUE(is_power_of_two(0));
}

TEST(Bithacks, NextPowerOfTwo) {
    EXPECT_EQ(round_power_of_two(2), 2);
    EXPECT_EQ(round_power_of_two(1024), 1024);
    EXPECT_EQ(round_power_of_two(3), 4);
    EXPECT_EQ(round_power_of_two(1337), 2048);

    // NB
    EXPECT_EQ(round_power_of_two(0), 0);
}

TEST(Bithacks, binary_log) {
    EXPECT_EQ(binary_log(16), 4);
    EXPECT_EQ(binary_log(1024), 10);
    EXPECT_EQ(binary_log(1024 * 1024 + 1024), 20);
    EXPECT_EQ(binary_log(1337), 10);
}

TEST(Bithacks, get_byte) {
    EXPECT_EQ(get_byte(0x1234567890ABCDEF, 0), 0xEF);
    EXPECT_EQ(get_byte(0x1234567890ABCDEF, 1), 0xCD);
    EXPECT_EQ(get_byte(0x1234567890ABCDEF, 2), 0xAB);
    EXPECT_EQ(get_byte(0x1234567890ABCDEF, 3), 0x90);
}

TEST(Bithacks, reverse_byte) {
    EXPECT_EQ(reverse_byte(0x00), 0x00);
    EXPECT_EQ(reverse_byte(0x01), 0x80);
    EXPECT_EQ(reverse_byte(0x80), 0x01);
    EXPECT_EQ(reverse_byte(0xFF), 0xFF);
}

TEST(ByteOperationsTest, reverse_bits) {
    EXPECT_EQ(reverse_bits(0x1234567890ABCDEF), 0xF7B3D5091E6A2C48);
    EXPECT_EQ(reverse_bits(0x0123456789ABCDEF), 0xF7B3D591E6A2C480);
    EXPECT_EQ(reverse_bits(0x00000000FFFFFFFF), 0xFFFFFFFF00000000);
    EXPECT_EQ(reverse_bits(0xFFFFFFFF00000000), 0x00000000FFFFFFFF);
}

TEST(ByteOperationsTest, next_lex_permutation) {
    EXPECT_EQ(next_lex_permutation(0x0), 0x7FFFFFFF);
}

TEST(Bithacks, get_low_half) {
    EXPECT_EQ(get_low_half(0x1234567890ABCDEF), 0x90ABCDEF);
    EXPECT_EQ(get_low_half(0x0000000012345678), 0x12345678);
    EXPECT_EQ(get_low_half(0xFFFFFFFF00000000), 0x00000000);
}

TEST(Bithacks, get_high_half) {
    EXPECT_EQ(get_high_half(0x1234567890ABCDEF), 0x12345678);
    EXPECT_EQ(get_high_half(0x0000000012345678), 0x0000000);
    EXPECT_EQ(get_high_half(0xFFFFFFFF00000000), 0xFFFFFFFF);
}
