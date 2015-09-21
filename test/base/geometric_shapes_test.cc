// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/geometric_shapes.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class CylinderTest : public ::testing::Test {
 protected:
  CylinderTest() {
    c_.set_diameter(10);
    c_.set_length(10);
  }

  Cylinder c_;
};

TEST_F(CylinderTest, AreaCrossSection) {
  EXPECT_EQ(78.54, helper::Round(c_.AreaCrossSection(), 2));
}

TEST_F(CylinderTest, Volume) {
  EXPECT_EQ(785.4, helper::Round(c_.Volume(), 1));
}
