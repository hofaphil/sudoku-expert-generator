#include "gtest/gtest.h"

extern "C" {
#include "../src/Number.h"
}

TEST(NumberTests, new_number)
{
  const int b = 3, n = 2;
  number number = new_number(b, n);
  EXPECT_EQ(number.block, b);
  EXPECT_EQ(number.number, n);
}