// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/helper.h"

#include "gtest/gtest.h"

TEST(Helper, Round) {
  const double value = 123.45;

  EXPECT_EQ(123, helper::Round(value, 0));
  EXPECT_EQ(123.5, helper::Round(value, 1));
  EXPECT_EQ(123.45, helper::Round(value, 2));
  EXPECT_EQ(123.450, helper::Round(value, 3));
}
