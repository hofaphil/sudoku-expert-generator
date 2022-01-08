#include "gtest/gtest.h"

extern "C" {
	#include "../src/Number.h"
}

TEST(NumberTests, new_number)
{
  const int b = 3, n = 2;
  number number = new_number(b, n);
  ASSERT_EQ(number.block, b);
  ASSERT_EQ(number.number, n);
}