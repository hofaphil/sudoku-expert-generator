#include "gtest/gtest.h"

extern "C" {
#include "../src/Block.h"
}

TEST(BlockTests, new_block)
{
  block block = new_block();

  for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++) {
	  EXPECT_EQ(block.numbers[i][j], 0);
	}

  for (int j = 0; j < 3; j++)
	for (int i = 0; i < 10; i++) {
	  EXPECT_EQ(block.contains_number[i], 0);
	  EXPECT_EQ(block.row[j][i], 0);
	  EXPECT_EQ(block.column[j][i], 0);
	}

  EXPECT_EQ(block.latest_del_index_x, -1);
  EXPECT_EQ(block.latest_del_index_x, block.latest_del_index_z);
}

TEST(BlockTests, new_block_copy)
{
  block block_orig = new_block();
  block block_copy = new_block_copy(block_orig);

  for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++) {
	  EXPECT_EQ(block_orig.numbers[i][j], block_copy.numbers[i][j]);
	}

  for (int j = 0; j < 3; j++)
	for (int i = 0; i < 10; i++) {
	  EXPECT_EQ(block_orig.contains_number[i], block_copy.contains_number[i]);
	  EXPECT_EQ(block_orig.row[j][i], block_copy.row[j][i]);
	  EXPECT_EQ(block_orig.column[j][i], block_copy.column[j][i]);
	}

  EXPECT_EQ(block_orig.latest_del_index_x, block_copy.latest_del_index_x);
  EXPECT_EQ(block_orig.latest_del_index_z, block_copy.latest_del_index_z);
}
