// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/vector.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

TEST(Vector2d, Angle) {

  Vector2d v(0,0);

  // quadrant I
  v.set_x(1);
  v.set_y(0);
  EXPECT_EQ(0, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(0, helper::Round(v.Angle(true), 0));

  v.set_x(1);
  v.set_y(1);
  EXPECT_EQ(45, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(45, helper::Round(v.Angle(true), 0));

  // quadrant II
  v.set_x(0);
  v.set_y(1);
  EXPECT_EQ(90, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(90, helper::Round(v.Angle(true), 0));

  v.set_x(-1);
  v.set_y(1);
  EXPECT_EQ(135, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(135, helper::Round(v.Angle(true), 0));

  // quadrant III
  v.set_x(-1);
  v.set_y(0);
  EXPECT_EQ(180, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(180, helper::Round(v.Angle(true), 0));

  v.set_x(-1);
  v.set_y(-1);
  EXPECT_EQ(225, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(-135, helper::Round(v.Angle(true), 0));

  // quadrant IV
  v.set_x(0);
  v.set_y(-1);
  EXPECT_EQ(270, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(-90, helper::Round(v.Angle(true), 0));

  v.set_x(1);
  v.set_y(-1);
  EXPECT_EQ(315, helper::Round(v.Angle(false), 0));
  EXPECT_EQ(-45, helper::Round(v.Angle(true), 0));
}

TEST(Vector2d, Magnitude) {

  Vector2d v(3, 4);
  EXPECT_EQ(5, helper::Round(v.Magnitude(), 0));
}

TEST(Vector2d, Rotate) {

  Vector2d v(1, 0);

  // positive rotation
  v.Rotate(135);
  EXPECT_EQ(135, helper::Round(v.Angle(false), 0));

  // negative rotation
  v.Rotate(-45);
  EXPECT_EQ(90, helper::Round(v.Angle(false), 0));

  // rotation over 360 degrees
  v.Rotate(720);
  EXPECT_EQ(90, helper::Round(v.Angle(false), 0));

  // magnitude unchanged
  EXPECT_EQ(1, helper::Round(v.Magnitude(), 0));
}

TEST(Vector2d, Scale) {

  Vector2d v(1, 0);

  v.Scale(100);
  EXPECT_EQ(100, helper::Round(v.Magnitude(), 0));

  v.Scale(.01);
  EXPECT_EQ(1, helper::Round(v.Magnitude(), 0));
}
