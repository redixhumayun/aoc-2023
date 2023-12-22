#include <gtest/gtest.h>

// Test for update_range function
TEST(UpdateRangeTest, HandlesGreaterOperation) {
  Range range = {1, 4000};
  range = update_range('x', '>', 1000, range);
  EXPECT_EQ(range.low, 1001);
  EXPECT_EQ(range.high, 4000);
}

TEST(UpdateRangeTest, HandlesLessOperation) {
  Range range = {1, 4000};
  range = update_range('x', '<', 3000, range);
  EXPECT_EQ(range.low, 1);
  EXPECT_EQ(range.high, 2999);
}

// Test for is_state_valid function
TEST(IsStateValidTest, ValidState) {
  BFS_State state = {"", {1, 100}, {1, 100}, {1, 100}, {1, 100}};
  EXPECT_TRUE(is_state_valid(state));
}

TEST(IsStateValidTest, InvalidState) {
  BFS_State state = {"", {100, 1}, {1, 100}, {1, 100}, {1, 100}};
  EXPECT_FALSE(is_state_valid(state));
}
