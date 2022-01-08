#include "gtest/gtest.h"

extern "C" {
	#include "Number.h"
}

TEST(NumberTests, new_number)
{
  const int b = 0, n = 0;
  number number = new_number(b, n);
  ASSERT_EQ(number.block, b);
  ASSERT_EQ(number.number, n);
}