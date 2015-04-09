// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "base/geometric_shapes.h"

#include <cmath>

#include "gtest/gtest.h"

class CylinderTest : public ::testing::Test {
 protected:
  CylinderTest() {
    c_.set_diameter(10);
    c_.set_length(10);
  }

  Cylinder c_;
};

TEST_F(CylinderTest, AreaCrossSection) {

  EXPECT_EQ(79, std::round(c_.AreaCrossSection()));
}

TEST_F(CylinderTest, Volume) {

  EXPECT_EQ(785, std::round(c_.Volume()));
}
