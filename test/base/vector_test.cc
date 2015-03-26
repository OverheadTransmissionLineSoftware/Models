// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "base/vector.h"

#include <cmath>

#include "gtest/gtest.h"

// This file tests the Base library, specifically the vector.cc file.

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

TEST(Vector2d, Magnitude) {

  Vector2d v(3, 4);
  EXPECT_EQ(5, std::round(v.Magnitude()));
}

TEST(Vector2d, Rotate) {

  Vector2d v(1, 0);

  // positive rotation
  v.Rotate(135);
  EXPECT_EQ(135, std::round(v.Angle(false)));

  // negative rotation
  v.Rotate(-45);
  EXPECT_EQ(90, std::round(v.Angle(false)));

  // rotation over 360 degrees
  v.Rotate(720);
  EXPECT_EQ(90, std::round(v.Angle(false)));

  // magnitude unchanged
  EXPECT_EQ(1, std::round(v.Magnitude()));
}

TEST(Vector2d, Scale) {

  Vector2d v(1, 0);

  v.Scale(100);
  EXPECT_EQ(100, std::round(v.Magnitude()));

  v.Scale(.01);
  EXPECT_EQ(1, std::round(v.Magnitude()));
}
