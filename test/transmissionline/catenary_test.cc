// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/catenary.h"

#include "gtest/gtest.h"

#include "base/convert_units.h"

class Catenary2dTest : public ::testing::Test {
 protected:
  Catenary2dTest() {

    Vector2d spacing_endpoints(1000, 0);

    c_.set_spacing_endpoints(spacing_endpoints);
    c_.set_tension_horizontal(1000);
    c_.set_weight_unit(0.5);
  }

  Catenary2d c_;
};

TEST_F(Catenary2dTest, Constant) {

  EXPECT_EQ(2000, supportfunctions::Round(c_.Constant(), 0));
}

TEST_F(Catenary2dTest, Length) {

  // flat - lowpoint within span
  EXPECT_EQ(1010.45, supportfunctions::Round(c_.Length(), 2));

  // inclined - lowpoint outside span
  Vector2d spacing_endpoints(1000, 500);
  c_.set_spacing_endpoints(spacing_endpoints);
  EXPECT_EQ(1127.39, supportfunctions::Round(c_.Length(), 2));
}

TEST_F(Catenary2dTest, LengthSlack) {

  // flat - lowpoint within span
  EXPECT_EQ(10.45, supportfunctions::Round(c_.LengthSlack(), 2));

  // inclined - lowpoint outside span
  Vector2d spacing_endpoints(1000, 500);
  c_.set_spacing_endpoints(spacing_endpoints);
  EXPECT_EQ(9.36, supportfunctions::Round(c_.LengthSlack(), 2));
}

TEST_F(Catenary2dTest, Tension) {

  EXPECT_EQ(1031.41, supportfunctions::Round(c_.Tension(0), 2));
  EXPECT_EQ(1000.00, supportfunctions::Round(c_.Tension(0.5), 2));
}

TEST_F(Catenary2dTest, TensionAverage) {

  // approximation method
  EXPECT_EQ(1010.54, supportfunctions::Round(c_.TensionAverage(0), 2));

  // numerical method
  EXPECT_EQ(1010.74, supportfunctions::Round(c_.TensionAverage(100), 2));
  EXPECT_EQ(1010.56, supportfunctions::Round(c_.TensionAverage(1000), 2));
}

TEST_F(Catenary2dTest, TensionMax) {

  // flat - lowpoint within span
  EXPECT_EQ(1031.41, supportfunctions::Round(c_.TensionMax(), 2));

  // inclined - lowpoint outside span
  Vector2d spacing_endpoints(1000, 500);
  c_.set_spacing_endpoints(spacing_endpoints);
  EXPECT_EQ(1275.78, supportfunctions::Round(c_.TensionMax(), 2));
}

TEST_F(Catenary2dTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}
