// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "base/vector.h"

#include <cmath>

#include "gtest/gtest.h"

// Tests the Angle() method.
TEST(Vector2d, Angle) {

  Vector2d v(0,0);

  // quadrant I
  v.set_x(1);
  v.set_y(0);
  EXPECT_EQ(0, std::round(v.Angle(false)));
  EXPECT_EQ(0, std::round(v.Angle(true)));

  v.set_x(1);
  v.set_y(1);
  EXPECT_EQ(45, std::round(v.Angle(false)));
  EXPECT_EQ(45, std::round(v.Angle(true)));

  // quadrant II
  v.set_x(0);
  v.set_y(1);
  EXPECT_EQ(90, std::round(v.Angle(false)));
  EXPECT_EQ(90, std::round(v.Angle(true)));

  v.set_x(-1);
  v.set_y(1);
  EXPECT_EQ(135, std::round(v.Angle(false)));
  EXPECT_EQ(135, std::round(v.Angle(true)));

  // quadrant III
  v.set_x(-1);
  v.set_y(0);
  EXPECT_EQ(180, std::round(v.Angle(false)));
  EXPECT_EQ(180, std::round(v.Angle(true)));

  v.set_x(-1);
  v.set_y(-1);
  EXPECT_EQ(225, std::round(v.Angle(false)));
  EXPECT_EQ(-135, std::round(v.Angle(true)));

  // quadrant IV
  v.set_x(0);
  v.set_y(-1);
  EXPECT_EQ(270, std::round(v.Angle(false)));
  EXPECT_EQ(-90, std::round(v.Angle(true)));

  v.set_x(1);
  v.set_y(-1);
  EXPECT_EQ(315, std::round(v.Angle(false)));
  EXPECT_EQ(-45, std::round(v.Angle(true)));
}
